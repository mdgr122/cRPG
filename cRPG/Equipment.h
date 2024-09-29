#pragma once
#include <string>

class Player; // Forward declaration of Player

struct StatModifier
{
	enum class ModifierType { STRENGTH = 0, SPEED, INTELLIGENCE, DEXTERITY, ELEMENTAL, NO_TYPE };
	enum class ElementalType { FIRE = 0, EARTH, WIND, ICE, WATER, LIGHTNING, NO_TYPE };

	int statModifierVal;
	ModifierType modifierType;
	ElementalType elementalType;
	std::wstring modifierTypeStr;

	StatModifier(int val = 0, ModifierType mod_type = ModifierType::NO_TYPE, ElementalType elemental_type = ElementalType::NO_TYPE)
		: statModifierVal{ val }
		, modifierType{ mod_type }
		, elementalType{ elemental_type }
	{
		switch (mod_type)
		{
		case StatModifier::ModifierType::STRENGTH:
			modifierTypeStr = L"Strength";
			break;
		case StatModifier::ModifierType::SPEED:
			modifierTypeStr = L"Speed";
			break;
		case StatModifier::ModifierType::INTELLIGENCE:
			modifierTypeStr = L"Intelligence";
			break;
		case StatModifier::ModifierType::DEXTERITY:
			modifierTypeStr = L"Dexterity";
			break;
		case StatModifier::ModifierType::ELEMENTAL:
			modifierTypeStr = L"Elemental";
			break;
		case StatModifier::ModifierType::NO_TYPE:
			modifierTypeStr = L"No_Type";
			break;
		default:
			break;
		}
	}
};

struct WeaponProperties
{
	enum class WeaponType { SWORD=0, DAGGER, BOW, STAFF, NOT_A_WEAPON};
	int attackPower;
	WeaponType weaponType;

	WeaponProperties(int attack_power = 0, WeaponType weapon_type = WeaponType::NOT_A_WEAPON)
		: attackPower{attack_power}, weaponType { weapon_type} {}
};

struct ArmourProperties
{
	enum class ArmourType { HEADGEAR=0, CHEST_BODY, FOOTWEAR, NOT_ARMOUR };
	int defensePower;
	ArmourType armourType;

	ArmourProperties(int defense_power = 0, ArmourType armour_type = ArmourType::NOT_ARMOUR)
		: defensePower{ defense_power }, armourType{ armour_type } {}
};

class Equipment
{
public:
	enum class EquipType { WEAPON=0, ARMOUR, ACCESSORY, NO_TYPE};

	virtual ~Equipment() {}
	virtual const int GetValue() const = 0;

	virtual bool OnEquip(Player& player) = 0;
	virtual bool OnRemove(Player& player) = 0;

	inline void Remove() { if (m_bEquipped) m_bEquipped = false; }
	inline void Equip() { if (!m_bEquipped) m_bEquipped = true; }
	inline const bool IsEquipped() const { return m_bEquipped; }
	
	inline const std::wstring& GetName() const { return m_sName; }
	inline const std::wstring& GetDescription() const { return m_sDescription; }
	
	inline bool Add(int num) 
	{ 
		if (m_Count + num > MAX_COUNT)
			return false;

		m_Count += num;
		return true;
	}
	
	inline const int GetCount() const { return m_Count; }

	inline const Equipment::EquipType GetType() const { return m_eEquipType; }
	inline const WeaponProperties& GetWeaponProperties() const { return m_WeaponProperties; }
	inline const ArmourProperties& GetArmourProperties() const { return m_ArmourProperties; }
	inline const StatModifier& GetStatModifier() const { return m_StatModifier; }





protected:
	std::wstring m_sName{ L"" }, m_sDescription{ L"" };
	int m_BuyPrice{ 0 }, m_SellPrice{ 0 }, m_Count{ 1 }, m_Weight{ 0 };
	bool m_bEquipped{ false };
	WeaponProperties m_WeaponProperties;
	ArmourProperties m_ArmourProperties;
	StatModifier m_StatModifier;

	void SetEquipType(EquipType type) { m_eEquipType = type; }

private:
	const int MAX_COUNT = 50;
	Equipment::EquipType m_eEquipType{ EquipType::NO_TYPE };


};

class Weapon : public Equipment
{
public:
	Weapon();
	Weapon(const std::wstring& name, const std::wstring& description, int buy_price, int sell_price,
		WeaponProperties weapon_properties = WeaponProperties(),
		StatModifier stat_modifier = StatModifier());
	
	~Weapon() = default;

	inline const int GetValue() const override { return m_WeaponProperties.attackPower; }
	bool OnEquip(Player& player) override;
	bool OnRemove(Player& player) override;

};

class Armour : public Equipment
{
public:
	Armour();
	Armour(const std::wstring& name, const std::wstring& description, int buy_price, int sell_price,
		ArmourProperties armour_properties = ArmourProperties(),
		StatModifier stat_modifier = StatModifier());
	~Armour() = default;

	inline const int GetValue() const override { return m_ArmourProperties.defensePower; }
	bool OnEquip(Player& player) override;
	bool OnRemove(Player& player) override;
};