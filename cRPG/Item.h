#pragma once
#include <string>

class Player;

class Item
{
public:
	enum ItemType { HEALTH = 0, STATUS_EFFECT, REVIVE, BATTLE, KEY_ITEM, NO_ITEM, NO_TYPE};

	virtual ~Item() {}
	virtual bool OnUse(Player& player) = 0;

	const int GetCount() const { return m_Count; }

	void Decrement()
	{
		if (m_Count > 0) 
			m_Count--;
		if (m_Count <= 0)
			m_Count = 0;
	}

	bool AddItem(int num) 
	{ 
		if (m_Count + num > MAX_COUNT)
			return false;
		m_Count += num;
		return true;
	}

	const std::wstring& GetItemName() const { return m_sItemName; }
	const std::wstring& GetItemDescription() const { return m_sItemDescription; }


protected:
	int m_Count{ 1 }, m_BuyPrice{ 0 }, m_SellPrice{ 0 }, m_ItemValue{ 0 };
	std::wstring m_sItemName{ L"Item_Name: " };
	std::wstring m_sItemDescription{ L"Item description here " };

	void SetType(ItemType type) { m_eItemType = type; }


private:
	const int MAX_COUNT = 99;
	ItemType m_eItemType;
};