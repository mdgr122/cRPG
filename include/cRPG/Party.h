#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Inventory.h"

class Player;

class Party
{
public:
    Party();
    ~Party();


    Inventory &GetInventory() { return m_Inventory; }
    std::vector<std::shared_ptr<Player>> &GetParty() { return m_PartyMembers; }


    const int GetGold() const { return m_Gold; }
    const int getNumActiveMembers() const { return m_NumActiveMembers; }

    void AddGold(int gold);

    bool AddMember(std::shared_ptr<Player> newMember);
    bool RemoveMember(const std::wstring &memberID, bool eliminate = false);
    bool BuyItem(int price, std::shared_ptr<Item> item);
    bool BuyEquipment(int price, std::shared_ptr<Equipment> equipment);

private:
    const int MAX_ACTIVE_MEMBERS = 4;
    const int MAX_MEMBERS        = 16;
    const int MAX_GOLD           = 99999;

    std::vector<std::shared_ptr<Player>> m_PartyMembers;
    Inventory m_Inventory;


    int m_Gold;
    int m_NumActiveMembers;
};
