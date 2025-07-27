#pragma once
#include <concepts>
#include <memory>
#include <string>
#include "../Equipment.h"
#include "../Item.h"
#include "../Logger.h"
#include "../Potion.h"

class ItemCreator
{
public:
    template <class... Args>
    static std::shared_ptr<Item> CreateItem(Item::ItemType type, Args &&... args)
    {
        switch (type)
        {
            case Item::ItemType::HEALTH:
                return Create<Potion>(std::forward<Args>(args)...);
            case Item::ItemType::REVIVE:
                return nullptr;
            //return Create<Revive>(std::forward<Args>(args) ...);
            case Item::ItemType::STATUS_EFFECT:
                return nullptr;
            //return Create<StatusEffect>(std::forward<Args>(args) ...);
            case Item::ItemType::BATTLE:
                return nullptr;
            //return Create<Battle>(std::forward<Args>(args) ...);
            case Item::ItemType::KEY_ITEM:
                return nullptr;
            //return Create<KeyItem>(std::forward<Args>(args) ...);
            case Item::ItemType::NO_TYPE:
                return nullptr;
            default:
                return nullptr;
        }
    }

    template <class... Args>
    static std::shared_ptr<Equipment> CreateEquipment(Equipment::EquipType type,
                                                      WeaponProperties weapon_properties,
                                                      ArmourProperties armour_properties,
                                                      StatModifier stat_modifier,
                                                      Args &&... args)
    {
        switch (type)
        {
            case Equipment::EquipType::WEAPON:
                return Create<Weapon>(std::forward<Args>(args)..., weapon_properties, stat_modifier);
            case Equipment::EquipType::ARMOUR:
                return Create<Armour>(std::forward<Args>(args)..., armour_properties, stat_modifier);
            case Equipment::EquipType::ACCESSORY:
                //TODO: return Create<Accessory>(std::forward<Args>(args) ...);
                return nullptr;
            case Equipment::EquipType::NO_TYPE: CRPG_ERROR("Equipment::EquipType == NONE");
                return nullptr;
            default: CRPG_ERROR("Equipment::EquipType == NONE");
                return nullptr;
        }
    }

private:
    template <typename T, typename... Args> requires std::constructible_from<T, Args...>
    static std::shared_ptr<T> Create(Args &&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
        // If the item we're trying to create is not constructible from the variable arguments we pass it, then this function won't be called by the compiler
    }

    // The compiler will check to see if the item is constructible from the variable arguments we pass it. If it works, it's going to return the shared pointer and forward those arguments into the shared pointer via the function above.
    // If the item is not constructible, then it will just return a null pointer.
    template <typename T, typename... Args>
    static std::shared_ptr<T> Create(Args &&... args)
    {
        return nullptr;
    }
};
