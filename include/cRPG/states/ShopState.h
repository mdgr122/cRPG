#pragma once
#include "InterfaceState.h"
#include "../Selector.h"

// Forward Declarations
class Party;
//class Player;
class Console;
class StateMachine;
class Keyboard;
struct ShopParameters;
class Equipment;
class Item;

class ShopState : public InterfaceState
{
public:
	ShopState(Party& party, Console& console, StateMachine& stateMachine, Keyboard& keyboard, const std::string& shopFilepath);
	~ShopState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	const int PANEL_BARS = 90;
	Console& m_Console;
	StateMachine& m_StateMachine;
	Keyboard& m_Keyboard;
	Party& m_Party;

	std::unique_ptr<ShopParameters> m_pShopParameters; // We are returning a unique pointer of shop parameters in the ShopLoader.cpp

	// By default, the selector is a wstring
	Selector<> m_ShopChoiceSelector;
	Selector<> m_BuySellSelector;
	Selector <std::shared_ptr<Equipment>> m_EquipmentSelector;
	Selector <std::shared_ptr<Item>> m_ItemSelector;

	// Need the price and quantity 
	int m_Quantity, m_Price, m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX, m_AvailableSellQuantity;
	bool m_bInShopSelect, m_bInItemBuy, m_bInItemSell, m_bSetFuncs, m_bIsEquipmentShop, m_bExitShop, m_bBuySellItem;

	void DrawShop();
	void DrawBuyItems();
	void DrawSellItems();
	void DrawItemsBox();
	void ResetSelections();

	void BuyEquipment();
	void SellEquipment();
	void BuyItems();
	void SellItems();

	// Selector functions -- Callback functions
	void OnShopMenuSelect(int index, std::vector<std::wstring> data);
	void BuySellOptions(int index, std::vector<std::wstring> data);

	// Buy is looking at shops inventory
	void OnBuyItemSelect(int index, std::vector<std::shared_ptr<Item>> data);
	void OnBuyEquipmentSelect(int index, std::vector<std::shared_ptr<Equipment>> data);

	// Sell is looking at party inventory
	void OnSellItemSelect(int index, std::vector<std::shared_ptr<Item>> data);
	void OnSellEquipmentSelect(int index, std::vector<std::shared_ptr<Equipment>> data);

	void RenderBuyItems(int x, int y, std::shared_ptr<Item> item);
	void RenderBuyEquipment(int x, int y, std::shared_ptr<Equipment> item);

	void RenderSellItems(int x, int y, std::shared_ptr<Item> item);
	void RenderSellEquipment(int x, int y, std::shared_ptr<Equipment> item);

	void UpdateBuyQuantity(int price);
	void UpdateSellQuantity(int totalAvailable);
};