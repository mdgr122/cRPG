#pragma once
#include <string>

class Player;

class Item
{
public:
	enum ItemType { HEALTH = 0, STATUS_EFFECT, REVIVE, BATTLE, KEY_ITEM, NO_ITEM, NO_TYPE};

	virtual ~Item() {}
	virtual bool OnUse(Player& player) = 0;

	const int Getcount() const { return m_Count; }

	void Decrement()
	{
		if (m_Count > 0) 
			m_Count--;
		if (m_Count <= 0)
			m_Count = 0;
	}

	void AddItem(int num)
	{
		m_Count += num;
	}

	const std::wstring& GetItemName() const { return m_sItemName; }
	const std::wstring& GetItemDescription() const { return m_sItemDescription; }


protected:
	int m_Count{ 1 };
	std::wstring m_sItemName{ L"Item_Name: " };
	std::wstring m_sItemDescription{ L"Item description here " };

	void SetType(ItemType type) { m_eItemType = type; }


private:
	ItemType m_eItemType;
};