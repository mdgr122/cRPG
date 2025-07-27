#pragma once
#include <memory>
#include "../Inventory.h"

struct ShopParameters
{
	enum class ShopType { WEAPON = 0, ARMOUR, ACCESSORY, ITEM, NOT_A_SHOP };
	std::shared_ptr<Inventory> inventory;
	ShopType shopType;

	ShopParameters(std::shared_ptr<Inventory> inventory, ShopType shopType = ShopType::ITEM) // Item shop by default
		: inventory{ std::move(inventory) }, shopType{ shopType }
	{

	}
	~ShopParameters() = default;
};