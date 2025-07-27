#pragma once
#include "Item.h"

class Player;

class Potion : public Item
{
public:
    Potion()
        : Potion(L"item_name", L"Item Description", 10, 10) {}

    Potion(const std::wstring &item_name, const std::wstring &description, int health = 10, int buy_price = 0);

    ~Potion() override;
    bool OnUse(Player &player) override;
};
