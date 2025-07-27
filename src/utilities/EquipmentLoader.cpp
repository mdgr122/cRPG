#include "utilities/EquipmentLoader.h"
#include <cassert>
#include "Logger.h"
#include "utilities/ItemCreator.h"
#include "utilities/crpg_utils.h"

using namespace tinyxml2;

EquipmentLoader::EquipmentLoader(const std::string &filepath, bool weapons)
    : m_sFilepath{filepath}
    , m_bWeaponLoader{weapons}
{
    m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

EquipmentLoader::~EquipmentLoader()
{}

std::shared_ptr<Equipment> EquipmentLoader::CreateObjectFromFile(const std::string &objName)
{
    if (LoadFile(m_sFilepath) != XML_SUCCESS)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to load the equipment XML file - " + m_sFilepath + " - " + error);
        return nullptr;
    }

    // If loaded file correctly, get the root element.
    XMLElement *pRootElement = m_pXMLDoc->RootElement();

    // Now check to see if root element is null ptr
    if (!pRootElement)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to get the root element for - " + error);
        return nullptr;
    }

    // Get the equipment, and check whether we're loading weapons or armour
    XMLElement *pEquipment = pRootElement->FirstChildElement(m_bWeaponLoader ? "Weapons" : "Arms");
    if (!pEquipment)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to get the equipment - " + error);
        return nullptr;
    }

    // Get the first item (weapon/armour/accessory)
    XMLElement *pItem = pEquipment->FirstChildElement(m_bWeaponLoader ? "Weapon" : "Armour");
    if (!pItem)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to get the first item - " + error);
        return nullptr;
    }

    std::shared_ptr<Equipment> newEquipment{nullptr};
    // Initialized to nullptr

    // Loop until we find the item we want to create
    while (pItem) // While the item is not nullptr
    {
        XMLElement *pName = pItem->FirstChildElement("Name");
        if (!pName)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get Item Name - " + error);
            return nullptr;
        }

        // Get the item name
        std::string name{pName->GetText()};
        if (name != objName)
        {
            pItem = pItem->NextSiblingElement(m_bWeaponLoader ? "Weapon" : "Armour");
            // Checks if we're loading weapon, if true then next sibling is a weapon, if not then next sibling is armour
            continue;
        }
        std::wstring equipName = CharToWide(name.c_str());


        // Get the type
        XMLElement *pType = pItem->FirstChildElement("Type");
        if (!pType)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get Item Type - " + error);
            return nullptr;
        }
        Equipment::EquipType equipType{EquipTypeFromString(pType->GetText())};


        // Get the item description
        XMLElement *pDesc = pItem->FirstChildElement("Description");
        if (!pDesc)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get Item Description - " + error);
            return nullptr;
        }
        std::wstring desc = CharToWide(pDesc->GetText());
        // Get the description as wide string

        // Get the buy price
        XMLElement *pBuyPrice = pItem->FirstChildElement("BuyPrice");
        if (!pBuyPrice)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get Item BuyPrice - " + error);
            return nullptr;
        }
        int buy_price = atoi(pBuyPrice->GetText());
        // Get the description as wide string

        if (buy_price < 1)
        {
            CRPG_ERROR("Failed to convert BuyPrice or BuyPrice does not exist in the XML file.");
            return nullptr;
        }

        // Get the sell price
        XMLElement *pSellPrice = pItem->FirstChildElement("SellPrice");
        if (!pSellPrice)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get Item SellPrice - " + error);
            return nullptr;
        }
        int sell_price = atoi(pSellPrice->GetText());
        // Get the description as wide string

        if (sell_price < 1)
        {
            CRPG_ERROR("Failed to convert SellPrice or SellPrice does not exist in the XML file.");
            return nullptr;
        }

        // Get the Equipment Properties
        WeaponProperties weapon_props = CreateWeaponProperties(pItem);
        ArmourProperties armour_props = CreateArmourProperties(pItem);
        StatModifier stat_mod         = CreateStatModifier(pItem);

        newEquipment = ItemCreator::CreateEquipment(equipType, weapon_props, armour_props, stat_mod, equipName, desc, buy_price, sell_price);

        return newEquipment;
    }

    // If we get to this point, then we did not create an item
    CRPG_ERROR("Item - [" + objName + "] does not exist!");
    assert(newEquipment);
    return nullptr;
}

WeaponProperties EquipmentLoader::CreateWeaponProperties(tinyxml2::XMLElement *xmlElement)
{
    XMLElement *pWeaponProps = xmlElement->FirstChildElement("WeaponProperties");
    if (!pWeaponProps)
        return WeaponProperties();
    // IF no weapon properties, send back an empty/default WeaponProperties object

    XMLElement *pAttackPower = pWeaponProps->FirstChildElement("AttackPower");
    int attackPower          = pAttackPower ? atoi(pAttackPower->GetText()) : 0;
    // Checking to see if nullptr, if false then we send back the attack power, if true then we just send back 0.

    // Get the weapon type
    XMLElement *pWeaponType           = pWeaponProps->FirstChildElement("WeaponType");
    const std::string weapon_type_str = pWeaponType ? std::string{pWeaponType->GetText()} : "";
    // Checking to see if nullptr, if false then we construct a string from the weapon text, if true then send back an empty string.
    WeaponProperties::WeaponType weapon_type = WeaponTypeFromString(weapon_type_str);

    return WeaponProperties(attackPower, weapon_type);
}

ArmourProperties EquipmentLoader::CreateArmourProperties(tinyxml2::XMLElement *xmlElement)
{
    XMLElement *pArmourProps = xmlElement->FirstChildElement("ArmourProperties");
    if (!pArmourProps)
        return ArmourProperties();
    // IF no weapon properties, send back an empty/default WeaponProperties object

    XMLElement *pDefensePower = pArmourProps->FirstChildElement("DefensePower");
    int defensePower          = pDefensePower ? atoi(pDefensePower->GetText()) : 0;
    // Checking to see if nullptr, if false then we send back the attack power, if true then we just send back 0.

    // Get the weapon type
    XMLElement *pArmourType           = pArmourProps->FirstChildElement("ArmourType");
    const std::string armour_type_str = pArmourType ? std::string{pArmourType->GetText()} : "";
    // Checking to see if nullptr, if false then we construct a string from the weapon text, if true then send back an empty string.
    ArmourProperties::ArmourType armour_type = ArmourTypeFromString(armour_type_str);

    return ArmourProperties(defensePower, armour_type);
}

StatModifier EquipmentLoader::CreateStatModifier(tinyxml2::XMLElement *xmlElement)
{
    XMLElement *pStatModifier = xmlElement->FirstChildElement("StatModifier");
    if (!pStatModifier)
        return StatModifier();
    // IF no weapon properties, send back an empty/default WeaponProperties object

    XMLElement *pModifierValue = pStatModifier->FirstChildElement("ModValue");
    int modifierValue          = pModifierValue ? atoi(pModifierValue->GetText()) : 0;
    // Checking to see if nullptr, if false then we send back the attack power, if true then we just send back 0.

    // Get the modifier type
    XMLElement *pModifierType           = pStatModifier->FirstChildElement("ModType");
    const std::string modifier_type_str = pModifierType ? std::string{pModifierType->GetText()} : "";
    // Checking to see if nullptr, if false then we construct a string from the weapon text, if true then send back an empty string.
    StatModifier::ModifierType modifier_type = ModifierTypeFromString(modifier_type_str);

    // Get elemental type
    XMLElement *pElementalType           = pStatModifier->FirstChildElement("ElementalType");
    const std::string elemental_type_str = pElementalType ? std::string{pElementalType->GetText()} : "";
    // Checking to see if nullptr, if false then we construct a string from the weapon text, if true then send back an empty string.
    StatModifier::ElementalType elemental_type = ElementalTypeFromString(elemental_type_str);


    return StatModifier(modifierValue, modifier_type, elemental_type);
}
