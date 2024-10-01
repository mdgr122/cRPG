#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Stats.h"
#include "Equipment.h"

class Actor
{
public:
	enum class ActorType
	{
		WARRIOR = 0, MAGE, THIEF,
	};

protected:
	std::wstring m_sName, m_sID;
	int m_Level, m_XP, m_XPToNextLevel, m_HP, m_MaxHP;

	bool m_bDead;
	ActorType m_eActorType;

	//std::unordered_map<Equipment::EquipType, std::shared_ptr<Equipment>> m_mapEquipment;
	std::unordered_map<Stats::EquipSlots, std::shared_ptr<Equipment>> m_mapEquipmentSlots;
	Stats m_Stats;

private:
	// Things that aren't going to change and won't be accessed
	const std::vector<std::wstring> m_EquipmentSlotLabels{L"Weapon", L"Armour", L"Headgear", L"Footwear", L"Accessory"};
	const std::vector<std::wstring> m_StatLabels{ L"Attack", L"Strength", L"Intelligence", L"Speed", L"Dexterity", L"Stamina" };

	// TODO: std::vector<std::wstring> m_StatLabels; 

public:
	Actor();
	Actor(const std::wstring& name, const std::wstring& id, int level, int max_hp, ActorType type = ActorType::WARRIOR); // Default Warrior Type
	~Actor() = default;

	inline const int GetHP() const { return m_HP; }
	inline const int GetMaxHP() const { return m_MaxHP; }

	inline const std::vector<std::wstring> GetEquipmentSlotLabels() const { return m_EquipmentSlotLabels; }
	inline const std::vector<std::wstring> GetStatLabels() const { return m_StatLabels; }

	// We want to use the equipment by polymorphism, so we're passing in a ptr to equipment that contains the child classes
	//std::unordered_map<Equipment::EquipType, std::shared_ptr<Equipment>> GetEquippedItems() { return m_mapEquipment; }
	std::unordered_map<Stats::EquipSlots, std::shared_ptr<Equipment>>& GetEquippedItemSlots() { return m_mapEquipmentSlots; }


	Stats& GetStats() { return m_Stats; }



	inline const bool IsDead() const { return m_bDead; }
	inline const std::wstring& GetName() const { return m_sName; }
	inline const std::wstring& GetID() const { return m_sID; }

	
	const int GetLevel() const { return m_Level; }
	const int GetXP() const { return m_XP; }
	const int GeXPToNextLevel() const { return m_XPToNextLevel; }

	void HealHP(int hp);
	void TakeDamage(int hp);

};