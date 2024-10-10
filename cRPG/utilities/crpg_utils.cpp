#include "crpg_utils.h"

std::wstring CharToWide(const char* str)
{
	return std::wstring(str, str + strlen(str));
}

std::string WideToStr(const std::wstring& wstr)
{
	// Uses windows function
	int size_in_bytes = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_in_bytes, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_in_bytes, NULL, NULL);
	return strTo;
}


// If num < 10, then adds a 0 wstring.
std::wstring PadNumbers(int num)
{
	if (num < 10)
		return L"0" + std::to_wstring(num);

	return std::to_wstring(num);
}

Item::ItemType ItemTypeFromString(const std::string& item_type)
{
	if (item_type == "Health")
		return Item::ItemType::HEALTH;
	else if (item_type == "Revive")
		return Item::ItemType::REVIVE;
	else if (item_type == "StatusEffect")
		return Item::ItemType::STATUS_EFFECT;
	else if (item_type == "Battle")
		return Item::ItemType::BATTLE;
	else if (item_type == "KeyItem")
		return Item::ItemType::KEY_ITEM;
	else
		return Item::ItemType::NO_TYPE;
}

Equipment::EquipType EquipTypeFromString(const std::string& equip_type)
{
	if (equip_type == "Weapon")
		return Equipment::EquipType::WEAPON;
	else if (equip_type == "Armour")
		return Equipment::EquipType::ARMOUR;
	else if (equip_type == "Accessory")
		return Equipment::EquipType::ACCESSORY;
	else
		return Equipment::EquipType::NO_TYPE;
}

ShopParameters::ShopType ShopTypeFromString(const std::string& shop_type)
{
	if (shop_type == "Item")
		return ShopParameters::ShopType::ITEM;
	else if (shop_type == "Accessory")
		return ShopParameters::ShopType::ACCESSORY;
	else if (shop_type == "Armour")
		return ShopParameters::ShopType::ARMOUR;
	else if (shop_type == "Weapon")
		return ShopParameters::ShopType::WEAPON;
	else
		return ShopParameters::ShopType::NOT_A_SHOP;
}

WeaponProperties::WeaponType WeaponTypeFromString(const std::string& weapon_type)
{
	if (weapon_type == "Sword")
		return WeaponProperties::WeaponType::SWORD;
	else if (weapon_type == "Dagger")
		return WeaponProperties::WeaponType::DAGGER;
	else if (weapon_type == "Staff")
		return WeaponProperties::WeaponType::STAFF;
	else if (weapon_type == "Bow")
		return WeaponProperties::WeaponType::BOW;
	else
		return WeaponProperties::WeaponType::NOT_A_WEAPON;
}

ArmourProperties::ArmourType ArmourTypeFromString(const std::string& armour_type)
{
	if (armour_type == "Headgear")
		return ArmourProperties::ArmourType::HEADGEAR;
	else if (armour_type == "Chest")
		return ArmourProperties::ArmourType::CHEST_BODY;
	else if (armour_type == "Footwear")
		return ArmourProperties::ArmourType::FOOTWEAR;
	else
		return ArmourProperties::ArmourType::NOT_ARMOUR;

}

StatModifier::ModifierType ModifierTypeFromString(const std::string& modifier_type)
{
	if (modifier_type == "Strength")
		return StatModifier::ModifierType::STRENGTH;
	else if (modifier_type == "Intelligence")
		return StatModifier::ModifierType::INTELLIGENCE;
	else if (modifier_type == "Dexterity")
		return StatModifier::ModifierType::DEXTERITY;
	else if (modifier_type == "Speed")
		return StatModifier::ModifierType::SPEED;
	else
		return StatModifier::ModifierType::NO_TYPE;
}

StatModifier::ElementalType ElementalTypeFromString(const std::string& elemental_type)
{
	if (elemental_type == "Earth")
		return StatModifier::ElementalType::EARTH;
	else if (elemental_type == "Fire")
		return StatModifier::ElementalType::FIRE;
	else if (elemental_type == "Ice")
		return StatModifier::ElementalType::ICE;
	else if (elemental_type == "Lightning")
		return StatModifier::ElementalType::LIGHTNING;
	else if (elemental_type == "Water")
		return StatModifier::ElementalType::WATER;
	else if (elemental_type == "Wind")
		return StatModifier::ElementalType::WIND;
	else
		return StatModifier::ElementalType::NO_TYPE;
}

