#include "utilities/ShopLoader.h"
#include "Logger.h"
#include "utilities/crpg_utils.h"
#include "utilities/EquipmentLoader.h"
#include "utilities/ItemCreator.h"
#include "utilities/ItemLoader.h"

using namespace tinyxml2;

ShopLoader::ShopLoader()
{
	m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

ShopLoader::~ShopLoader()
{}

std::unique_ptr<ShopParameters> ShopLoader::CreateShopParametersFromFile(
	const std::string& shop_filepath )
{
	if (LoadFile(shop_filepath) != XMLError::XML_SUCCESS)
	{
		CRPG_ERROR(
			"Failed to load shop parameters from [" + shop_filepath + "]");
		return nullptr;
	}

	// Get root element
	XMLElement* pRootElement = m_pXMLDoc->RootElement();

	// We always want to check if the ptr returned is a null ptr
	if (!pRootElement)
	{
		CRPG_ERROR("Failed to get the root element");
		return nullptr;
	}

	// Get shop params
	XMLElement* pShopParams = pRootElement->FirstChildElement("ShopParameters");
	if (!pShopParams)
	{
		CRPG_ERROR("Failed to get the root ShopParameters");
		return nullptr;
	}

	XMLElement* pShopType = pShopParams->FirstChildElement("ShopType");
	if (!pShopType)
	{
		CRPG_ERROR("Failed to get the root ShopType");
		return nullptr;
	}

	std::string shopTypeStr = pShopType->GetText();

	XMLElement* pInventory = pShopParams->FirstChildElement("Inventory");
	if (!pShopType)
	{
		CRPG_ERROR("Failed to get the root Inventory");
		return nullptr;
	}

	// Unique ptr to an inventory
	std::unique_ptr<Inventory> inventory = std::make_unique<Inventory>();
	ShopParameters::ShopType   shopType  = ShopTypeFromString(shopTypeStr);

	std::string definitionLocation{ "" };
	bool        itemLoader{ false };
	bool        weaponLoader{ false };

	switch (shopType)
	{
		case ShopParameters::ShopType::WEAPON:
			definitionLocation = "Assets/xml_files/WeaponDefs.xml";
			weaponLoader = true;
			break;
		case ShopParameters::ShopType::ARMOUR:
			definitionLocation = "Assets/xml_files/ArmourDefs.xml";
			break;
		case ShopParameters::ShopType::ACCESSORY:
			break;
		case ShopParameters::ShopType::ITEM:
			definitionLocation = "Assets/xml_files/ItemDefs.xml";
			itemLoader = true;
			break;
		case ShopParameters::ShopType::NOT_A_SHOP: CRPG_ERROR(
				"ShopType is not a known type")
			return nullptr;
	}

	// Create the item loader if item shop
	std::unique_ptr<ItemLoader> itemLoaderPtr{ nullptr };
	if (itemLoader) itemLoaderPtr = std::make_unique<ItemLoader>(
		definitionLocation);

	// Create the equipment loader if eqiupment shop
	std::unique_ptr<EquipmentLoader> equipmentLoaderPtr{ nullptr };
	if (!itemLoader) equipmentLoaderPtr = std::make_unique<EquipmentLoader>(
		definitionLocation,
		weaponLoader);

	// Get the shop items
	XMLElement* pShopItem = pInventory->FirstChildElement("ShopItem");
	if (!pShopItem)
	{
		CRPG_ERROR("Failed to get the shop items");
		return nullptr;
	}

	// Get the first item of the inventory
	XMLElement* pItem = pShopItem->FirstChildElement(shopTypeStr.c_str());
	if (!pItem)
	{
		CRPG_ERROR("Failed to get the first item");
		return nullptr;
	}

	// If first item successful then go into while loop
	while (pItem)
	{
		// Get item name
		XMLElement* pName = pItem->FirstChildElement("Name");
		if (!pName)
		{
			CRPG_ERROR("Failed to get the item name");
			return nullptr;
		}
		std::string name{ pName->GetText() }; // The name from the shop

		if (itemLoader)
		{
			// Load Items - Attempt to get item from item loader
			auto newItem = itemLoaderPtr->CreateObjectFromFile(name);

			if (newItem) inventory->AddItem(std::move(newItem));
		}
		else // Load equipment
		{
			auto newEquipment = equipmentLoaderPtr->CreateObjectFromFile(name);
			if (newEquipment) inventory->AddEquipment(std::move(newEquipment));
		}

		// Move to the next shop item
		pItem = pItem->NextSiblingElement(shopTypeStr.c_str());
	}

	// Create the ShopParameters
	std::unique_ptr<ShopParameters> shop_parameters = std::make_unique<
		ShopParameters>(std::move(inventory), shopType);
	if (!shop_parameters)
	{
		CRPG_ERROR("Failed to create the shop parameters");
		return nullptr;
	}

	return shop_parameters;
}

std::shared_ptr<ShopParameters> ShopLoader::CreateObjectFromFile(
	const std::string& objName )
{
	return std::shared_ptr<ShopParameters>();
}
