#pragma once
#include <memory>
#include <vector>
#include "Equipment.h"
#include "Item.h"

class Player;

class Inventory
{
public:
    Inventory();
    ~Inventory() = default;

    std::vector<std::shared_ptr<Item>> &GetItems() { return m_Items; }

    std::vector<std::shared_ptr<Equipment>> &GetEquipment()
    {
        return m_Equipment;
    }

    bool AddItem(std::shared_ptr<Item> newItem);
    bool AddEquipment(std::shared_ptr<Equipment> newEquipment);
    bool UseItem(int index, Player &player);

private:
    std::vector<std::shared_ptr<Item>> m_Items;
    std::vector<std::shared_ptr<Equipment>> m_Equipment;
};
