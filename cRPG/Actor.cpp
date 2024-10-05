#include "Actor.h"

Actor::Actor()
	: Actor(L"Actor_name", L"1", 1, 100, 200, ActorType::WARRIOR)
{

}

Actor::Actor(const std::wstring& name, const std::wstring& id, int level, int max_hp, int max_mp, ActorType type)
	: m_sName{ name }
	, m_sID{ id }
	, m_Level{ level }
	, m_XP{ 0 }
	, m_XPToNextLevel{ 250 }
	, m_HP{ max_hp }
	, m_MaxHP{ max_hp }
	, m_MP{ max_mp }
	, m_MaxMP{ max_mp }
	, m_bDead{ false }
	, m_eActorType{ type }
	, m_mapEquipmentSlots{ // Initializing the unordered_map of EquipmentSlots so we don't get mem errors if nothing is equipped
		{ Stats::EquipSlots::WEAPON, nullptr },
		{ Stats::EquipSlots::HEADGEAR, nullptr },
		{ Stats::EquipSlots::CHEST_BODY, nullptr },
		{ Stats::EquipSlots::FOOTWEAR, nullptr },
		{ Stats::EquipSlots::ACCESSORY, nullptr }
	}
	, m_Stats{ 10, 3, 5, 5, 7 } // temp testing stats
{

}

void Actor::HealHP(int hp)
{
	m_HP += hp;
	if (m_HP >= m_MaxHP)
		m_HP = m_MaxHP;
}

void Actor::TakeDamage(int hp)
{
	m_HP -= hp;
	if (m_HP <= 0)
	{
		m_HP = 0;
		m_bDead = true;
	}
}

bool Actor::useMP(int mp)
{
	if (mp > m_MP)
	{
		return false;
	}
	m_MP -= mp;

	if (m_MP <= 0)
	{
		m_MP = 0;
	}
	return true;
}
