#include <iostream>
#include "Stats.h"
#include "Logger.h"
#include "utilities/crpg_utils.h"


Stats::Stats()
	: Stats(0, 0, 0, 0, 0)
{
}

Stats::Stats(int strength, int intelligence, int speed, int dexterity, int stamina)
	: m_StatList{
		{L"Attack", 0},
		{L"Defense", 0},
		{L"Magic", 0},
		{L"Strength", strength},
		{L"Intelligence", intelligence},
		{L"Speed", speed},
		{L"Dexterity", dexterity},
		{L"Stamina", stamina}
	},
	m_StatModifierList{
		{L"Strength", 0},
		{L"Intelligence", 0},
		{L"Speed", 0},
		{L"Dexterity", 0},
		{L"Stamina", 0}
	},
	m_EquipSlotList{
		{EquipSlots::WEAPON, 0},
		{EquipSlots::HEADGEAR, 0},
		{EquipSlots::CHEST_BODY, 0},
		{EquipSlots::FOOTWEAR, 0},
		{EquipSlots::ACCESSORY, 0},
	}
{
	// Initialize the Attack and defense
	UpdateStats();
}

const int Stats::GetStat(const std::wstring& key)
{
	if (m_StatList.find(key) == m_StatList.end())
	{
		//std::string key_str = WideToStr(key);
		//CRPG_ERROR("Error: " + key_str);
		CRPG_ERROR("[" + WideToStr(key) + "] is not a valid Stat!");
		//std::wcout << L"[" << key << L"] is not a valid stat\n";
		return 0;
	}
	return m_StatList[key];
}

const int Stats::GetModifier(const std::wstring& key)
{
	if (m_StatModifierList.find(key) == m_StatModifierList.end())
	{
		// Temporarily diabling this log bc stat modifiers haven't been created yet, and it's creating log msg's every tick
		//CRPG_ERROR("[" + WideToStr(key) + "] is not a valid Stat Modifier!");
		return 0;
	}
	return m_StatModifierList[key];
}

const int Stats::GetEquipmentValue(EquipSlots slot)
{
	if (m_EquipSlotList.find(slot) == m_EquipSlotList.end())
	{
		//CRPG_ERROR("[" + key + "] is not a valid stat")
		std::wcout << L"Invalid slot!\n";
		return 0;
	}
	return m_EquipSlotList[slot];
}

void Stats::SetModifier(const std::wstring& key, int value)
{
	if (m_StatModifierList.find(key) == m_StatModifierList.end())
	{
		//std::wcout << L"[" << key << L"] is not a valid stat modifier\n";
		CRPG_ERROR("[" + WideToStr(key) + "] is not a valid Stat Modifier!");
		return;
	}
	m_StatModifierList[key] = value;
}

void Stats::SetEquipmentValue(EquipSlots slot, int value)
{
	if (m_EquipSlotList.find(slot) == m_EquipSlotList.end())
	{
		std::wcout << L"Invalid slot!\n";
		return;
	}
	m_EquipSlotList[slot] = value;
}

void Stats::SetStat(const std::wstring& key, int value)
{
	if (m_StatList.find(key) == m_StatList.end())
	{
		//CRPG_ERROR("[" + key + "] is not a valid stat");
		std::wcout << L"[" << key << L"] is not a valid stat\n";
		return;
	}
	m_StatList[key] = value;
}

void Stats::UpdateStats()
{
	// Calculate base stats with modifiers
	int baseStrength = m_StatList[L"Strength"] + m_StatModifierList[L"Strength"];
	int baseIntelligence = m_StatList[L"Intelligence"] + m_StatModifierList[L"Intelligence"];
	int baseDexterity = m_StatList[L"Dexterity"] + m_StatModifierList[L"Dexterity"];
	int baseSpeed = m_StatList[L"Speed"] + m_StatModifierList[L"Speed"];

	// Update attack power stat
	m_StatList[L"Attack"] = m_EquipSlotList[EquipSlots::WEAPON] +
		baseStrength +
		(baseIntelligence / 5) +
		(baseDexterity / 5);

	// Update defense power stat
	int totalArmourDefense = (m_EquipSlotList[EquipSlots::HEADGEAR] +
		m_EquipSlotList[EquipSlots::CHEST_BODY] +
		m_EquipSlotList[EquipSlots::FOOTWEAR]);

	m_StatList[L"Defense"] = totalArmourDefense +
		(baseStrength / 5) +
		(baseIntelligence / 5) +
		(baseSpeed / 5) +
		(baseDexterity / 5);
}

//void Stats::UpdateStats()
//{
//	// Update attack power stat
//	m_StatList[L"Attack"] = m_EquipSlotList[EquipSlots::WEAPON] +
//		(m_StatList[L"Strength"] + m_StatModifierList[L"Strength"]) +
//		((m_StatList[L"Intelligence"] + m_StatModifierList[L"Intelligence"]) / 5) +
//		((m_StatList[L"Dexterity"] + m_StatModifierList[L"Dexterity"]) / 5);
//
//	// Update defense power stat
//	m_StatList[L"Defense"] = m_EquipSlotList[EquipSlots::HEADGEAR] + m_EquipSlotList[EquipSlots::CHEST_BODY] + m_EquipSlotList[EquipSlots::FOOTWEAR] +
//		((m_StatList[L"Strength"] + m_StatModifierList[L"Strength"]) / 5) +
//		((m_StatList[L"Intelligence"] + m_StatModifierList[L"Intelligence"]) / 5) +
//		((m_StatList[L"Speed"] + m_StatModifierList[L"Speed"]) / 5) +
//		((m_StatList[L"Dexterity"] + m_StatModifierList[L"Dexterity"]) / 5);
//}
 