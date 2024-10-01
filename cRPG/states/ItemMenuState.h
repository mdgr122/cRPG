#pragma once
#include "InterfaceState.h"
#include "../Selector.h"
#include "../Item.h"

class Party;
class Console;
class StateMachine;
class Keyboard;
class Player;

class ItemState : public InterfaceState
{
public:
	ItemState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard);
	~ItemState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	const int PANEL_BARS = 90;
	const int MENU_SIZE = 27;
	const int SMALL_PANEL_BAR = 29;

	//Party& m_Party;
	Console& m_Console;
	StateMachine& m_StateMachine;
	Keyboard& m_Keyboard;
	Player& m_Player;

	Selector<> m_MenuSelector;
	Selector<std::shared_ptr<Item>> m_ItemSelector;

	bool m_bExitGame, m_bInMenuSelect;
	int m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX;

	enum class ItemChoice {ITEM = 0, KEY_ITEM};
	enum class SelectType { DRAW, PROCESS_INPUTS, HIDE, SHOW }; // hide = hidecursor, show = showcursor

	void DrawInventory();
	void DrawPlayerInfo();
	void SelectorFunc(int index, SelectType type); 
	void OnMenuSelect(int index, std::vector<std::wstring> data); // What menu are we selecting
	void OnItemSelect(int index, std::vector<std::shared_ptr<Item>> data); // Vector of shared pointers of items -- Wil include the inventory of items
	void RenderItem(int x, int y, std::shared_ptr<Item> item); // draw item checks to see if wstring, if not then we need to override.
	void FocusOnMenu();
};