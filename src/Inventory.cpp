#include "Inventory.h"
#include "Logger.h"
#include "utilities/crpg_utils.h"

Inventory::Inventory()
{}


// Function that goes through vector of current items the player/party has. If true, then we want to increment that item count. If false, we want to push the new item into the vector.
bool Inventory::AddItem(std::shared_ptr<Item> newItem)
{
    for (auto &item : m_Items)
    {
        // If we try and add the item, and the count exceeds the MAX_COUNT of 99, then it will return false;
        // if GetItemName() == GetItemName(), then item already exists, so increment.
        // If true, then check if item exceeds MAX_COUNT with bool AddItem(), then GetCount()
        if (item->GetItemName() == newItem->GetItemName())
            return item->AddItem(newItem->GetCount());
    }

    // If item is a newItem, then push newItem onto the m_Items inventory vector.
    m_Items.push_back(std::move(newItem));
    return true;
}

bool Inventory::AddEquipment(std::shared_ptr<Equipment> newEquipment)
{
    for (auto &equipment : m_Equipment)
    {
        // If we try and add the item, and the count exceeds the MAX_COUNT of 99, then it will return false;
        // if GetItemName() == GetItemName(), then item already exists, so increment.
        // If true, then check if item exceeds MAX_COUNT with bool AddItem(), then GetCount()
        if (equipment->GetName() == newEquipment->GetName())
            return equipment->Add(newEquipment->GetCount());
    }

    // If item is a newItem, then push newItem onto the m_Items inventory vector.
    m_Equipment.push_back(std::move(newEquipment));
    return true;
}

bool Inventory::UseItem(int index, Player &player)
{
    if (m_Items.empty())
    {
        CRPG_ERROR("Cannot use item when it does not exist!");
        return false;
    }

    if (index >= m_Items.size())
    {
        CRPG_ERROR("Failed to use item. Index is beyond item vector size - INDEX[" + std::to_string(index) + "]");
        return false;
    }

    // If item used, then decrement the m_items[index] of that item by 1, or however many were used.
    m_Items[index]->OnUse(player);
    if (m_Items[index]->GetCount() <= 0)
        remove(m_Items, index);

    return true;
}
