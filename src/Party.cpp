#include "Party.h"

#include <algorithm>
#include "Logger.h"
#include "Player.h"
#include "utilities/crpg_utils.h"

Party::Party()
    : m_PartyMembers{ }
    , m_Inventory{ }
    , m_Gold{1000}
    , m_NumActiveMembers{0}
{}

Party::~Party()
{}

bool Party::AddMember(std::shared_ptr<Player> newMember)
{
    if (m_PartyMembers.size() >= MAX_MEMBERS)
    {
        //CRPG_ERROR("There are already the maximum amount of players in this party." + " " + "[" + std::to_string(MAX_MEMBERS) + "]");
        CRPG_ERROR("There are already the maximum amount of players in this party.");
        return false;
    }

    // Check to see if the member is already in the party via the Player/ActorID
    // If ID is present in the vector, will return false, if not then true.
    for (const auto &member : m_PartyMembers)
    {
        if (member->GetID() == newMember->GetID())
        {
            //CRPG_ERROR("Member is already in the party!" + " " + "Member ID:" + " " + std::to_string(newMember));
            CRPG_ERROR("Member is already in the party!");
            return false;
        }
    }

    // Check to see if # of active members is < number of members in use. If True, then we need to set them as active and push them onto that vector.
    if (m_NumActiveMembers < MAX_ACTIVE_MEMBERS)
    {
        newMember->SetActive(true);
        m_NumActiveMembers++;
        newMember->SetPartyPosition(m_NumActiveMembers);
    }

    m_PartyMembers.push_back(std::move(newMember));
    // Move member into the m_PartyMembers vector, and because it is a shared pointer, we can copy or move them, so we're going to move the new member in.
    return true;
}

bool Party::RemoveMember(const std::wstring &memberID, bool eliminate)
{
    for (int i = 0; i < m_PartyMembers.size(); i++)
    {
        auto &member = m_PartyMembers[i];
        if (member->GetID() != memberID)
            continue;

        member->SetActive(false);
        m_NumActiveMembers--;

        if (eliminate)
            remove(m_PartyMembers, i);

        return true;
    }

    // CRPG_ERROR("Failed to remove the Party Member!" + " " + "MemberID: " + " " + memberID);
    CRPG_ERROR("Failed to remove the Party Member!");
    return false;
}

void Party::AddGold(int gold)
{
    m_Gold += gold;

    m_Gold = std::min(m_Gold, MAX_GOLD);
}

bool Party::BuyItem(int price, std::shared_ptr<Item> item)
{
    if (m_Gold <= price)
        return false;

    if (!m_Inventory.AddItem(item))
    {
        //CRPG_ERROR("Failed to add item to inventory!" + " " + "Item" + ": " + std::to_wstring(item));
        CRPG_ERROR("Failed to add item to inventory!");
        return false;
    }
    m_Gold -= price;

    m_Gold = std::max(m_Gold, 0);

    return true;
}

bool Party::BuyEquipment(int price, std::shared_ptr<Equipment> equipment)
{

    //if (m_Gold <= price)
    //{
    //    CRPG_LOG("Gold: " + std::to_string(m_Gold) + ", Base Price: " + std::to_string(price) + ", Quantity: " + std::to_string(equipment->GetCount()), + ", Total: " + std::to_string(price));
    //    return false;
    //}

    if (m_Gold <= 0 &&  m_Gold < price)
    {
        CRPG_LOG("Failed to Purchase Equipment!"); 
        CRPG_LOG("Gold: " + std::to_string(m_Gold) + " ,Base Price: " + std::to_string(price) + " ,Quantity: " + std::to_string(equipment->GetCount()), + " ,Total: " std::to_string(price));
        return false;
    }

    if (!m_Inventory.AddEquipment(equipment))
    {
        //CRPG_ERROR("Failed to add equipment to inventory!" + " " + "Equipment" + ": " + std::to_wstring(equipment));
        CRPG_ERROR("Failed to add equipment to inventory!");
        return false;
    }

    m_Gold -= price;

    m_Gold = (std::max)(m_Gold, 0);

    return true;
}
