#include "Party.h"

Party::Party()
{
}

Party::~Party()
{
}

bool Party::AddMember(std::shared_ptr<Player> newMember)
{
	return false;
}

bool Party::RemoveMember(const std::wstring& memberID, bool eliminate)
{
	return false;
}

void Party::AddGold(int gold)
{
}

bool Party::BuyItem(int price, std::shared_ptr<Item> item)
{
	return false;
}

bool Party::BuyEquipment(int price, std::shared_ptr<Equipment> equipment)
{
	return false;
}
