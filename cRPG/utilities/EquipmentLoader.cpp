#include "EquipmentLoader.h"
#include "crpg_utils.h"
#include "ItemCreator.h"
#include "../Logger.h"
#include <cassert>

using namespace tinyxml2;

EquipmentLoader::EquipmentLoader(const std::string& filepath, bool weapons)
	: m_sFilepath{filepath}
	, m_bWeaponLoader{weapons}
{
	m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

EquipmentLoader::~EquipmentLoader()
{
}

std::shared_ptr<Equipment> EquipmentLoader::CreateObjectFromFile(const std::string& objName)
{
	return std::shared_ptr<Equipment>();
}

WeaponProperties EquipmentLoader::CreateWeaponProperties(tinyxml2::XMLElement* xmlElement)
{
	XMLElement* pWeaponProps = xmlElement->FirstChildElement("WeaponProperties");
	if (!pWeaponProps)
		return WeaponProperties(); // IF no weapon properties, send back an empty/default WeaponProperties object

	XMLElement* pAttackPower = pWeaponProps->FirstChildElement("AttackPower");
	int attackPower = pAttackPower ? atoi(pAttackPower->GetText()) : 0; // Checking to see if nullptr, if false then we send back the attack power, if true then we just send back 0.

	// Get the weapon type
	XMLElement* pWeaponType = pWeaponProps->FirstChildElement("WeaponType");
	const std::string weapon_type_str = pWeaponType ? std::string{ pWeaponType->GetText() } : ""; // Checking to see if nullptr, if false then we construct a string from the weapon text, if true then send back an empty string.
	WeaponProperties::WeaponType type = WeaponTypeFromString(weapon_type_str);

	return WeaponProperties(attackPower, type);

}

ArmourProperties EquipmentLoader::CreateArmourProperties(tinyxml2::XMLElement* xmlElement)
{
	return ArmourProperties();
}

StatModifier EquipmentLoader::CreateStatModifier(tinyxml2::XMLElement* xmlElement)
{
	return StatModifier();
}
