#include "GameMenuState.h"
#include "ItemMenuState.h"
#include "EquipmentMenuState.h"
#include "../Party.h"
#include "../Player.h"
#include "../Console.h"
#include "../Keyboard.h"
#include "../States/StateMachine.h"
#include "../Selector.h"
#include "../utilities/Globals.h"

using namespace std::placeholders;

GameMenuState::GameMenuState(Party& party, Console& console, StateMachine& stateMachine, Keyboard& keyboard)
	: m_Party(party)
	, m_Console(console)
	, m_StateMachine(stateMachine)
	, m_Keyboard(keyboard)
	, m_MenuSelector{ 
		console,
		keyboard,
		{L"Items", L"Magic", L"Equipment", L"Stats", L"Order", L"Save", L"Exit"},
		//SelectorParams{30, 8, 1, 0, 4}
		SelectorParams{30, 8, 1, 0, 0}
	} // x, y, columns, currentX, currentY, spacingX, spacingY
	, m_PlayerSelector{
		console,
		keyboard,
		std::bind(&GameMenuState::OnPlayerSelect, this, _1, _2), // _1, _2, std::bind placeholder args.
		std::bind(&GameMenuState::OnDrawPlayerSelect, this, _1, _2, _3), // _1, _2, std::bind placeholder args.
		party.GetParty(),
		//SelectorParams{69, 13, 1, 0, 10} 
		SelectorParams{69, 13, 1, 0, 0, 10, 10} // x=69, y=13, columns=1, current_x=0, current_y=0, spacing_x=20, spacing_y=10
		//SelectorParams{69, 13, 1, 0, 0}
	}
	, m_bExitGame{false}
	, m_bInMenuSelect{true}
	, m_ScreenWidth{console.GetScreenWidth()}
	, m_ScreenHeight{console.GetScreenHeight()}
	, m_CenterScreenW{console.GetHalfWidth()}
	, m_PanelBarX{m_CenterScreenW - (PANEL_BARS / 2)}
	, m_eSelectType{SelectType::NONE}
{
	m_MenuSelector.SetSelectionFunc(std::bind(&GameMenuState::OnMenuSelect, this, _1, _2));
}


GameMenuState::~GameMenuState()
{
}

void GameMenuState::OnEnter()
{
	if (m_bInMenuSelect)
		m_PlayerSelector.HideCursor();

	m_Console.ClearBuffer();
}

void GameMenuState::OnExit()
{
	m_Console.ClearBuffer();
}

void GameMenuState::Update()
{
}

void GameMenuState::Draw()
{
	DrawPanels();
	DrawPlayerInfo();

	m_MenuSelector.Draw();
	m_PlayerSelector.Draw();

	m_Console.Draw();
}

void GameMenuState::ProcessInputs()
{
	if (m_bInMenuSelect)
	{
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
		{
			m_StateMachine.PopState();
			return;
		}

		m_MenuSelector.ProcessInputs(); // Only want to process the menu selector if we are infact in the menu selector
	}
	else
	{
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
		{
			m_PlayerSelector.HideCursor();
			m_bInMenuSelect = true;
			m_eSelectType = SelectType::NONE;
			m_MenuSelector.ShowCursor();
			m_Console.ClearBuffer();
			return;
		}

		m_PlayerSelector.ProcessInputs(); // Only want to select player selector if we're in player selection
	}
}

bool GameMenuState::Exit()
{
	return m_bExitGame;
}

void GameMenuState::DrawPanels()
{

	// Draw opening bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, BLUE); // TOP BAR
	int menu_x_pos = m_CenterScreenW - (MENU_SIZE / 2);
	m_Console.Write(menu_x_pos, 2, L"  ____         _ ___                 ", GREEN);
	m_Console.Write(menu_x_pos, 3, L" |_  /___ __ _| | __|__ _ _ __ _ ___ ", GREEN);
	m_Console.Write(menu_x_pos, 4, L"  / // -_) _` | | _/ _ \\ '_/ _` / -_)", GREEN);
	m_Console.Write(menu_x_pos, 5, L" /___\\___\\__,_|_|_|\\___/_| \\__, \\___|", GREEN);
	m_Console.Write(menu_x_pos, 6, L"                           |___/     ", GREEN);
	m_Console.DrawPanelHorz(18, 7, PANEL_BARS, BLUE); // BOTTOM MENU HORIZONTAL

	m_Console.DrawPanelHorz(m_PanelBarX - 1, 40, SMALL_PANEL_BAR, BLUE); // SMALL UNDER SAVE
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, BLUE); // BOTTOM LAST

	m_Console.DrawPanelVert(m_PanelBarX - 1, 2, 44, BLUE); // LEFT
	m_Console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, BLUE); // RIGHT
	m_Console.DrawPanelVert(48, 8, 38, BLUE); // MIDDLE

	// Draw the game time
	const auto& time_str = L"TIME: " + CRPG_Globals::GetInstance().GetTime();
	m_Console.Write(26, 44, time_str);
}

void GameMenuState::DrawPlayerInfo()
{
	int i = 0;
	for (const auto& player : m_Party.GetParty())
	{
		// If player not actively in party, then we want to ignore the non-active players
		if (!player->isActive())
			continue;

		// Get the player attributes
		const auto& name = player->GetName();
		const auto& hp = std::to_wstring(player->GetHP());
		const auto& hp_max = std::to_wstring(player->GetMaxHP());
		const auto& level = std::to_wstring(player->GetLevel());
		const auto& xp = std::to_wstring(player->GetXP());
		const auto& xp_to_next = std::to_wstring(player->GeXPToNextLevel());

		//std::wstring hp_string = L"HP: " + hp + L" / " + hp_max;
		//std::wstring level_string = L"Level: " + level + L" " + L"XP: " + xp + L" / " + xp_to_next;
		std::wstring hp_string = L"HP: " + hp + L" / " + hp_max;
		std::wstring level_string = L"Lvl: " + level + L" Exp: " + xp + L" / " + xp_to_next;


		m_Console.Write(75, 12 + i, name, LIGHT_AQUA);
		m_Console.Write(75, 13 + i, hp_string, LIGHT_AQUA);
		m_Console.Write(75, 14 + i, level_string, LIGHT_AQUA);
		i += 10;
	}

	// Draw party gold
	//const auto& gold = std::to_wstring(m_Party->GetGold());
	const auto& gold = std::to_wstring(m_Party.GetGold());

	std::wstring gold_string = L"GOLD: " + gold;
	m_Console.Write(26, 42, gold_string);
}

void GameMenuState::OnMenuSelect(int index, std::vector<std::wstring> data)
{
	switch (index)
	{
	case 0: m_eSelectType = SelectType::ITEM; break;
	case 1: m_eSelectType = SelectType::MAGIC; break;
	case 2: m_eSelectType = SelectType::EQUIPMENT; break;
	case 3: m_eSelectType = SelectType::STATS; break;
	case 4: m_eSelectType = SelectType::ORDER; break;
	case 5:
		// TODO: Save Game
		return;
	case 6: m_bExitGame = true; return;
	default: return;
	}

	if (m_eSelectType != SelectType::NONE)
	{
		m_MenuSelector.HideCursor();
		m_PlayerSelector.ShowCursor();
		m_bInMenuSelect = false;
	}
}

void GameMenuState::OnPlayerSelect(int index, std::vector<std::shared_ptr<Player>> data)
{
	const auto& player = data[index]; 
	switch (m_eSelectType)
	{
	case SelectType::ITEM:
		m_StateMachine.PushState(std::make_unique<ItemState>(*player, m_Console, m_StateMachine, m_Keyboard));
		break;
	case SelectType::MAGIC:
		// TODO: Create new Magic State
		break;
	case SelectType::EQUIPMENT:
		m_StateMachine.PushState(std::make_unique<EquipmentMenuState>(*player, m_Console, m_StateMachine, m_Keyboard));
		break;
	case SelectType::STATS:
		// TODO: Create new Stats State
		break;
	case SelectType::ORDER:
		// TODO: Create new Order State
		break;
	default:
		break;
	}
}

void GameMenuState::OnDrawPlayerSelect(int x, int y, std::shared_ptr<Player> player)
{
	/* NOTHING TO DO HERE -- JUST A PLACEHOLDER */
}
