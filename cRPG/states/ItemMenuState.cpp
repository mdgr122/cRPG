#include "ItemMenuState.h"
#include "../Console.h"
#include "../Player.h"
#include "StateMachine.h"
#include "../Keyboard.h"
#include "../utilities/crpg_utils.h"

using namespace std::placeholders;

ItemState::ItemState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard)
	: m_Console(console)
	, m_StateMachine(stateMachine)
	, m_Keyboard(keyboard)
	, m_Player(player)
	, m_MenuSelector{
		console, keyboard,
		{L"Items", L"Key Items"},
		//SelectorParams{40, 10, 2, 25, 0}
		SelectorParams{40, 10, 2, 0, 0} // x, y, cols, current x, current y
	}
	, m_ItemSelector{
		console, keyboard,
		std::bind(&ItemState::OnItemSelect, this, _1, _2),
		std::bind(&ItemState::RenderItem, this, _1, _2, _3),
		std::vector<std::shared_ptr<Item>>(),
		SelectorParams{30, 14, 2, 0, 0}
		//SelectorParams{40, 12, 2, 26, 1}
	}
	, m_bExitGame{ false }
	, m_bInMenuSelect{true}
	, m_ScreenWidth{console.GetScreenWidth()}
	, m_ScreenHeight{console.GetScreenHeight()}
	, m_CenterScreenW{console.GetHalfWidth()}
	, m_PanelBarX{ m_CenterScreenW - (PANEL_BARS / 2) }

{
	m_MenuSelector.SetSelectionFunc(std::bind(&ItemState::OnMenuSelect, this, _1, _2));
	m_ItemSelector.SetData(m_Player.GetInventory().GetItems());
}

ItemState::~ItemState()
{
}

void ItemState::OnEnter()
{
	CRPG_LOG("Entered ItemMenuState")
	m_Console.ClearBuffer();
}

void ItemState::OnExit()
{
	CRPG_LOG("Exited ItemMenuState")
	m_Console.ClearBuffer();
}

void ItemState::Update()
{
}

void ItemState::Draw()
{
	m_Console.Draw();
	DrawInventory();
	DrawPlayerInfo();

	m_MenuSelector.Draw();

	if (!m_bInMenuSelect)
		SelectorFunc(m_MenuSelector.GetIndex(), SelectType::DRAW);
}

void ItemState::ProcessInputs()
{
	// Checks to see if we're in menu select. If true, then process the menu selector inputs.
	// Also checks to see if backspace was just pressed, if true then pop the menu select state to exit that state.
	if (m_bInMenuSelect)
	{
		m_MenuSelector.ProcessInputs();
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
		{
			m_StateMachine.PopState();
		}
	}
	else
	{
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
		{ 
			FocusOnMenu();
			m_Console.ClearBuffer(); // Clear buffer bc
		}
		SelectorFunc(m_MenuSelector.GetIndex(), SelectType::PROCESS_INPUTS);
	}
}

bool ItemState::Exit()
{
	return false;
}

void ItemState::DrawInventory()
{
	// Draw Opening Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, BLUE);
	int menu_x_pos = m_CenterScreenW - (MENU_SIZE / 2);
	m_Console.Write(menu_x_pos, 2, L"  ___ _                     ", GREEN);
	m_Console.Write(menu_x_pos, 3, L" |_ _| |_ ___ _ __ ___  ___ ", GREEN);
	m_Console.Write(menu_x_pos, 4, L"  | || __/ _ \\ '_ ` _ \\/ __|", GREEN);
	m_Console.Write(menu_x_pos, 5, L"  | || ||  __/ | | | | \\__ \\", GREEN);
	m_Console.Write(menu_x_pos, 6, L" |___|\\__\\___|_| |_| |_|___/", GREEN);
	m_Console.DrawPanelHorz(18, 7, PANEL_BARS, BLUE);

	m_Console.DrawPanelHorz(m_PanelBarX - 1, 9, PANEL_BARS + 2, BLUE);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 11, PANEL_BARS + 2, BLUE);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 13, PANEL_BARS + 2, BLUE);

	// Player holder
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 10), PANEL_BARS + 2, BLUE);
	// Draw Closing Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, BLUE);

	// Draw Right bar
	m_Console.DrawPanelVert(m_PanelBarX - 1, 2, 44, BLUE);
	m_Console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, BLUE);
}

void ItemState::DrawPlayerInfo()
{
	// Get Players info
	const auto& name = m_Player.GetName();
	const auto& hp = m_Player.GetHP();
	const auto& hp_max = m_Player.GetMaxHP();
	const auto& level = std::to_wstring(m_Player.GetLevel());
	const auto& xp = std::to_wstring(m_Player.GetXP());
	const auto& xp_to_next = std::to_wstring(m_Player.GeXPToNextLevel());

	// Change the color of HP based on health percentage
	WORD hp_color = WHITE;
	if (hp <= hp_max * 0.3f)
		hp_color = RED;
	else if (hp <= hp_max * 0.6)
		hp_color = YELLOW;

	std::wstring hp_string = L"HP: " + std::to_wstring(hp) + L" / " + std::to_wstring(hp_max);
	std::wstring level_string = L"Level: " + level + L" XP: " + xp + L" / " + xp_to_next;

	m_Console.Write(35, 3 + m_ScreenHeight - 10, name);
	m_Console.Write(35, 4 + m_ScreenHeight - 10, hp_string);
	m_Console.Write(35, 5 + m_ScreenHeight - 10, level_string);

}

void ItemState::SelectorFunc(int index, SelectType type)
{
	switch (static_cast<ItemChoice>(index))
	{
	case ItemChoice::ITEM:
	{
		if (type == SelectType::PROCESS_INPUTS)
			m_ItemSelector.ProcessInputs();
		else if (type == SelectType::DRAW)
			m_ItemSelector.Draw();
		else if (type == SelectType::SHOW)
			m_ItemSelector.ShowCursor();
		else if (type == SelectType::HIDE)
			m_ItemSelector.HideCursor();
		break;
	}
	case ItemChoice::KEY_ITEM:
	{
		// TODO
		break;
	}
	default:
		break;
	}
}

void ItemState::OnMenuSelect(int index, std::vector<std::wstring> data)
{
	m_MenuSelector.HideCursor(); // Ghides the menu select cursor
	m_bInMenuSelect = false; // sets this to false
	SelectorFunc(index, SelectType::SHOW); // selector for either the item or key item, enters into that ItemChoice
}

void ItemState::OnItemSelect(int index, std::vector<std::shared_ptr<Item>> data)
{
	auto& items = m_Player.GetInventory();

	if (items.GetItems().empty())
	{
		CRPG_LOG("items.GetItems() is empty!");
		return;
	}
	
	//CRPG_LOG("items.GetItems() is not empty!");
	// Item is actually used in Inventory.cpp
	items.UseItem(index, m_Player);

	// Get item count
	const auto item_count = data[index]->GetCount();
	std::cout << "Item Count: " << item_count << std::endl;
	
	if (item_count <= 0)
	{
		remove(data, index);
		m_ItemSelector.SetData(items.GetItems());

		// Clear buffer
		m_Console.ClearBuffer();
	}
}

void ItemState::RenderItem(int x, int y, std::shared_ptr<Item> item)
{
	int index = m_ItemSelector.GetIndex();
	const auto& data = m_ItemSelector.GetData();

	const std::wstring& item_name = item->GetItemName();
	m_Console.Write(x, y, item_name);
	m_Console.Write(x + static_cast<int>(item_name.size() + 1), y, std::to_wstring(item->GetCount()));

	if (index < data.size())
	{
		const std::wstring& item_desc = item->GetItemDescription();
		m_Console.Write(m_CenterScreenW - static_cast<int>(item_desc.size() / 2), 12, item_desc, LIGHT_BLUE); // Getting center of screen and subtracting half of the size of the item_desc to center it in the screen.
	}

}


// Sets game menu back to true -- It hides the item selector and shows the menu selector.
// This is called if backspace is pressed in ProcessInputs
void ItemState::FocusOnMenu()
{
	m_bInMenuSelect = true;
	SelectorFunc(m_MenuSelector.GetIndex(), SelectType::HIDE); // Hides Item Selector bc we don't want to see it when not in that area
	m_MenuSelector.ShowCursor();
}
