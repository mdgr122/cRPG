#include "Player.h"

Player::Player(const std::wstring& name, const std::wstring& id, Inventory& inventory, int level, int max_hp, int max_mp)
	: Actor(name, id, level, max_hp, max_mp)  // Initialize base class Actor with name, id, level, and max_hp
	, m_Inventory(inventory) // Initialize Player's m_Inventory member with the inventory object passed as a parameter (Set the inventory to the inventory passed into the player, and that will be passed from the party before the player is added to the party)
	, m_bActive{ false }  // Initialize m_bActive to false
	, m_PartyPosition{ 0 }  // Initialize m_PartyPosition to 0
{

}


Player::~Player()
{
}
