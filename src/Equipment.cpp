#include "Equipment.h"
#include "Logger.h"
#include "Player.h"

Weapon::Weapon()
    : Weapon(L"Weapon_Name", L"Weapon Description", 0, 0)
{}

Weapon::Weapon(const std::wstring &name, const std::wstring &description, int buy_price, int sell_price, WeaponProperties weapon_properties, StatModifier stat_modifier)
{
    m_sName        = name;
    m_sDescription = description;
    m_BuyPrice     = buy_price;
    m_SellPrice    = sell_price;
    m_StatModifier = stat_modifier;
    SetEquipType(EquipType::WEAPON);
    m_WeaponProperties = weapon_properties;
    m_ArmourProperties = ArmourProperties();
    // Set a weapon to an empty armour properties which is 0 defensive power
    //m_Value = m_WeaponProperties.attackPower;
}

bool Weapon::OnEquip(Player &player)
{
    const auto &item_power = GetValue();
    auto &player_stats     = player.GetStats();

    player_stats.SetEquipmentValue(Stats::EquipSlots::WEAPON, item_power);


    // Set the weapon power
    const auto &stat_modifier = GetStatModifier();

    if (stat_modifier.modifierType != StatModifier::ModifierType::NO_TYPE)
        player_stats.SetModifier(stat_modifier.modifierTypeStr, stat_modifier.statModifierVal);


    // equip the item
    Equip();

    player_stats.UpdateStats();
    return true;
}

bool Weapon::OnRemove(Player &player)
{
    auto &player_stats = player.GetStats();
    player_stats.SetEquipmentValue(Stats::EquipSlots::WEAPON, 0);

    const auto &stat_modifier = GetStatModifier();

    if (stat_modifier.modifierType != StatModifier::ModifierType::NO_TYPE)
        player_stats.SetModifier(stat_modifier.modifierTypeStr, 0);

    Remove();
    player_stats.UpdateStats();

    return true;
}

Armour::Armour()
    : Armour(L"Armour_Name", L"Armour Description", 0, 0)
{}

Armour::Armour(const std::wstring &name, const std::wstring &description, int buy_price, int sell_price, ArmourProperties armour_properties, StatModifier stat_modifier)
{
    m_sName        = name;
    m_sDescription = description;
    m_BuyPrice     = buy_price;
    m_SellPrice    = sell_price;
    m_StatModifier = stat_modifier;
    SetEquipType(EquipType::ARMOUR);
    m_ArmourProperties = armour_properties;
    m_WeaponProperties = WeaponProperties();
    // Set armour to an empty weapon property which is 0 attack power
}

bool Armour::OnEquip(Player &player)
{
    const auto &item_power = GetValue();
    auto &player_stats     = player.GetStats();

    //player_stats.SetEquipmentValue(Stats::EquipSlots::, item_power);

    Stats::EquipSlots slot = Stats::EquipSlots::NO_SLOT;

    switch (m_ArmourProperties.armourType)
    {
        case ArmourProperties::ArmourType::HEADGEAR:
            slot = Stats::EquipSlots::HEADGEAR;
            break;
        case ArmourProperties::ArmourType::CHEST_BODY:
            slot = Stats::EquipSlots::CHEST_BODY;
            break;
        case ArmourProperties::ArmourType::FOOTWEAR:
            slot = Stats::EquipSlots::FOOTWEAR;
            break;
        case ArmourProperties::ArmourType::NOT_ARMOUR:
            slot = Stats::EquipSlots::NO_SLOT;
            return false;
        default:
            slot = Stats::EquipSlots::NO_SLOT;
            return false;
    }

    // Get the stat modifier
    const auto &stat_modifier = GetStatModifier();

    if (stat_modifier.modifierType != StatModifier::ModifierType::NO_TYPE)
        player_stats.SetModifier(stat_modifier.modifierTypeStr, stat_modifier.statModifierVal);

    player_stats.SetEquipmentValue(slot, item_power);

    Equip();
    player_stats.UpdateStats();

    return true;
}

bool Armour::OnRemove(Player &player)
{
    auto &player_stats = player.GetStats();

    Stats::EquipSlots slot = Stats::EquipSlots::NO_SLOT;
    switch (m_ArmourProperties.armourType)
    {
        case ArmourProperties::ArmourType::HEADGEAR:
            slot = Stats::EquipSlots::HEADGEAR;
            break;
        case ArmourProperties::ArmourType::CHEST_BODY:
            slot = Stats::EquipSlots::CHEST_BODY;
            break;
        case ArmourProperties::ArmourType::FOOTWEAR:
            slot = Stats::EquipSlots::FOOTWEAR;
            break;
        case ArmourProperties::ArmourType::NOT_ARMOUR:
            slot = Stats::EquipSlots::NO_SLOT;
            return false;
        default:
            slot = Stats::EquipSlots::NO_SLOT;
            return false;
    }

    // Get the stat modifier
    const auto &stat_modifier = GetStatModifier();

    if (stat_modifier.modifierType != StatModifier::ModifierType::NO_TYPE)
        player_stats.SetModifier(stat_modifier.modifierTypeStr, 0);

    player_stats.SetEquipmentValue(slot, 0);

    Remove();
    player_stats.UpdateStats();

    return true;
}
