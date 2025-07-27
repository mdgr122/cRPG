#include "states/EquipmentMenuState.h"
#include "Player.h"
#include "Console.h"
#include "states/StateMachine.h"
#include "Keyboard.h"
#include <cassert>

using namespace std::placeholders;

EquipmentMenuState::EquipmentMenuState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard)
	: m_Console{console}
	, m_StateMachine{stateMachine}
	, m_Keyboard{keyboard}
	, m_Player{player}
	, m_MenuSelector{
		console, keyboard,
		{L"EQUIP", L"REMOVE", L"OPTIMIZE"},
		//SelectorParams{42, 10, 3, 15, 2}
		SelectorParams{38, 10, 3, 0, 0} // X, Y, COLS, CURR_X, CURR_Y [X of text, Y of text, col of text, x of selector, y of selector]
	}
	, m_EquipSlotSelector{
		console, keyboard,
		std::bind(&EquipmentMenuState::OnSlotSelect, this, _1, _2),
		std::bind(&EquipmentMenuState::RenderEquipSlots, this, _1, _2, _3), // this func
		player.GetEquipmentSlotLabels(),
		//SelectorParams{30, 15, 1, 2, 1} // x, y, columns, currentX, currentY, spacingX, spacingY;
		//SelectorParams{30, 15, 1, 2, 2, 2} // x and y coords -- x=20, y=10 means 20 in and 10 down in terminal - Old cursor ->
		SelectorParams{30, 15, 1, 0, 0}
	}
	, m_EquipmentSelector{
		console, keyboard,
		std::bind(&EquipmentMenuState::OnEquipSelect, this, _1, _2),
		std::bind(&EquipmentMenuState::RenderEquip, this, _1, _2, _3),
		player.GetInventory().GetEquipment(),
		//SelectorParams{30, 34, 3, 20, 2}

		// Controls both the selector of the item once we enter the the equipslotslector, by hitting space on weapon/headgera/armour/footwear/accessory/etc.
		SelectorParams{30, 34, 2, 0, 0} // X, Y, COLS, CURR_X, CURR_Y [X of text, Y of text, col of text, x of selector, y of selector]
	}
	, m_bExitGame{false}
	, m_bInMenuSelect{true}
	, m_bInSlotSelect{false}
	, m_bRemoveEquipment{false}
	, m_ScreenWidth{console.GetScreenWidth()}
	, m_ScreenHeight{console.GetScreenHeight()}
	, m_CenterScreenW{console.GetHalfWidth()}
	, m_PanelBarX{ m_CenterScreenW - (PANEL_BARS / 2) }
	, m_DiffPosY{0}
	, m_PrevStatModPos{0}
	, m_PrevIndex{-1}
	, m_sCurrentSlot{L"NO_SLOT"}
	, m_eEquipSlots{Stats::EquipSlots::NO_SLOT}

{
	m_MenuSelector.SetSelectionFunc(std::bind(&EquipmentMenuState::OnMenuSelect, this, _1, _2));
	m_EquipmentSelector.HideCursor(); // Selectors are shown by default, so we want to hide the EquipmentSelector explicitly
	m_EquipSlotSelector.HideCursor(); // Selectors are shown by default, so we want to hide the EquipSlotSelector explicitly
}


EquipmentMenuState::~EquipmentMenuState()
{
}

void EquipmentMenuState::OnEnter()
{
	m_Console.ClearBuffer();
}

void EquipmentMenuState::OnExit()
{
	m_Console.ClearBuffer();
}

void EquipmentMenuState::Update()
{

	UpdateIndex();
}

void EquipmentMenuState::Draw()
{
	DrawEquipment();
	DrawPlayerInfo();

	// Drawing menu selecor
	m_MenuSelector.Draw();

	// Drawing equipment slots
	m_EquipSlotSelector.Draw();

	// If not in slot or menu select
	if (!m_bInSlotSelect && !m_bInMenuSelect)
	{
		DrawStatPrediction();
		m_EquipmentSelector.Draw();
	}

	m_Console.Draw();
}

void EquipmentMenuState::ProcessInputs()
{
	if (m_bInMenuSelect)
	{
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
		{
			m_StateMachine.PopState();
			return;
		}

		m_MenuSelector.ProcessInputs();
	}
	else if (m_bInSlotSelect)
	{
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
		{
			m_MenuSelector.ShowCursor();
			m_EquipSlotSelector.HideCursor();

			m_bInMenuSelect = true;
			m_bInSlotSelect = false;
			m_bRemoveEquipment = false;
			m_Console.ClearBuffer();
			return;
		}
		m_EquipSlotSelector.ProcessInputs();
	}
	else
	{
		if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE)) // equipment slot selector - working
		{
			m_EquipSlotSelector.ShowCursor();
			m_EquipmentSelector.HideCursor();

			m_bInMenuSelect = false;
			m_bInSlotSelect = true;
			m_Console.ClearBuffer();
			return;
		}

		m_EquipmentSelector.ProcessInputs();
	}
}

bool EquipmentMenuState::Exit()
{
	return false;
}

void EquipmentMenuState::DrawEquipment()
{
	// Draw Opening Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, YELLOW);
	int menu_x_pos = m_CenterScreenW - (EQUIP_SIZE / 2);
	m_Console.Write(menu_x_pos, 2, L" _____            _                            _  ", GREEN);
	m_Console.Write(menu_x_pos, 3, L"| ____|__ _ _   _(_)_ __  _ __ ___   ___ _ __ | |_ ", GREEN);
	m_Console.Write(menu_x_pos, 4, L"|  _| / _` | | | | | '_ \\| '_ ` _ \\ / _ \\ '_ \\| __|", GREEN);
	m_Console.Write(menu_x_pos, 5, L"| |__| (_| | |_| | | |_) | | | | | |  __/ | | | |_", GREEN);
	m_Console.Write(menu_x_pos, 6, L"|_____\\__, |\\__,_|_| .__/|_| |_| |_|\\___|_| |_|\\__|", GREEN);
	m_Console.Write(menu_x_pos, 7, L"         |_|       |_|                             ", GREEN);

	m_Console.DrawPanelHorz(m_PanelBarX - 1, 9, PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 11, PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 13, PANEL_BARS + 2, YELLOW);

	// Player holder
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 15), PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 17), PANEL_BARS + 2, YELLOW);
	// Draw Closing Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, YELLOW);

	// Draw Right bar
	m_Console.DrawPanelVert(m_PanelBarX - 1, 2, 44, YELLOW);
	m_Console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, YELLOW);

	m_Console.Write(30, 21, L"ATTRIBUTES", YELLOW);
	m_Console.Write(30, 22, L"==========", YELLOW);
}

void EquipmentMenuState::DrawPlayerInfo()
{
	const auto& player_name = m_Player.GetName();
	m_Console.Write(m_CenterScreenW - static_cast<int>(player_name.size() / 2), 12, player_name);

	const auto& stats_list = m_Player.GetStats().GetStatList();
	int i = 0;

	// structured binding -- statslist is an unordered map
	for (const auto& [stat, value] : stats_list)
	{
		const auto& mod_value = m_Player.GetStats().GetModifier(stat); // The GetModifier() func takes in a string which is a key to the actual value
		m_Console.Write(STAT_LABEL_X_POS, STAT_LABEL_START_Y_POS + i, stat); // Writing the stat to the buffer
		m_Console.Write(STAT_VAL_X_POS, STAT_LABEL_START_Y_POS + i, std::to_wstring(value + mod_value));
		DrawStatModifier(STAT_PREDICT_X_POS, STAT_LABEL_START_Y_POS + i, stat, value);
		i++;
	}
}

void EquipmentMenuState::DrawStatPrediction()
{
	if (m_bInSlotSelect)
		return;

	const auto& data = m_EquipmentSelector.GetData();

	if (data.empty())
		return;

	const auto& index = m_EquipmentSelector.GetIndex();
	if (index > data.size() - 1)
	{
		return;
	}

	const auto& item = data[index];

	if (item->IsEquipped())
		return;

	// Getting current equipped item so we can compare the new item vs the current item to see if stats are increasing or decreasing
	const auto& current_equipped = m_Player.GetEquippedItemSlots()[m_eEquipSlots];

	int current_equipped_val = 0;
	if (current_equipped)
	{
		current_equipped_val = current_equipped->GetValue();
	}

	const auto& new_item_val = item->GetValue();
	const auto& name = item->GetName();

	int difference = new_item_val - current_equipped_val;

	WORD diff_color = WHITE;

	std::wstring diff_direction = L"=";
	if (difference > 0)
	{
		diff_color = GREEN;
		diff_direction = L"+";
	}
	else if (difference < 0)
	{
		diff_color = RED;
		diff_direction = L"-";
	}

	int abs_diff_val = abs(difference);

	m_Console.Write(STAT_PREDICT_X_POS, m_DiffPosY, diff_direction + L" " + std::to_wstring(abs_diff_val), diff_color);
}

void EquipmentMenuState::DrawStatModifier(int x, int y, const std::wstring& stat, int value)
{
	if (m_bInSlotSelect)
	{
		return;
	}

	//std::wcout << stat << std::endl;
	const auto& data = m_EquipmentSelector.GetData();

	if (data.empty())
	{
		return;
	}

	const auto& index = m_EquipmentSelector.GetIndex();
	if (index > data.size() - 1)
	{
		m_Console.Write(x, m_PrevStatModPos, L"      ");
		return;
	}

	const auto& item = data[index];

	if (item->IsEquipped())
		return;

	const auto& stat_modifier = item->GetStatModifier();
	if (stat_modifier.modifierType == StatModifier::ModifierType::NO_TYPE)
		return;

	if (stat != stat_modifier.modifierTypeStr)
	{
		m_Console.Write(x, y, L"      ");
		return;
	}

	const std::wstring stat_val = std::to_wstring(stat_modifier.statModifierVal);
	m_Console.Write(x, y, L" + " + stat_val, GREEN);
	m_PrevStatModPos = y;
}

void EquipmentMenuState::OnMenuSelect(int index, std::vector<std::wstring> data)
{
	if (index > data.size() - 1 || data.empty())
		return;

	switch (index)
	{
	case 0:
		// Nothing here
		break;
	case 1:
		m_bRemoveEquipment = true;
		break;
	case 2:
		// TODO:
		break;
	default:
		return;
	}

	m_bInMenuSelect = false;
	m_bInSlotSelect = true;
	m_MenuSelector.HideCursor();
	m_EquipSlotSelector.ShowCursor();
}

void EquipmentMenuState::OnEquipSelect(int index, std::vector<std::shared_ptr<Equipment>> data)
{
	if (index > data.size() - 1 || data.empty())
		return;

	// If there is an item equipped in t hat slot, remove it (shouldn't be able to equipped 2 items to the  same slot, so we remove it first without making player have to do it manually)
	const auto& equippedItem = m_Player.GetEquippedItemSlots()[m_eEquipSlots];
	if (equippedItem)
		equippedItem->Remove();

	const auto& item = data[index];


	// First set the equip slots to nullptr to prevent overwriting other mem space, then set the null mem to the indexed item
	m_Player.GetEquippedItemSlots()[m_eEquipSlots] = nullptr;
	m_Player.GetEquippedItemSlots()[m_eEquipSlots] = item;

	Equipment::EquipType equipType = Equipment::EquipType::NO_TYPE;
	ArmourProperties::ArmourType armour_type = ArmourProperties::ArmourType::NOT_ARMOUR;
	//WeaponProperties::WeaponType weapon_type = WeaponProperties::WeaponType::NOT_A_WEAPON;

	switch (m_eEquipSlots)
	{
	case Stats::EquipSlots::WEAPON:
		equipType = Equipment::EquipType::WEAPON;
		break;
	case Stats::EquipSlots::HEADGEAR:
		equipType = Equipment::EquipType::ARMOUR;
		armour_type = ArmourProperties::ArmourType::HEADGEAR;
		break;
	case Stats::EquipSlots::CHEST_BODY:
		equipType = Equipment::EquipType::ARMOUR;
		armour_type = ArmourProperties::ArmourType::CHEST_BODY;
		break;
	case Stats::EquipSlots::FOOTWEAR:
		equipType = Equipment::EquipType::ARMOUR;
		armour_type = ArmourProperties::ArmourType::FOOTWEAR;
		break;
	case Stats::EquipSlots::ACCESSORY:
		equipType = Equipment::EquipType::ACCESSORY;
		break;
	default:
		break;
	}

	if (!item->OnEquip(m_Player))
	{
		CRPG_ERROR("Failed to equip the item on player")
			return;
	}

	// Reset the selected equipment -- this removes the item from the 'items to be selected' as it's already equipped
	std::vector<std::shared_ptr<Equipment>> selectedEquipment{};

	for (const auto& item : m_Player.GetInventory().GetEquipment())
	{
		const auto& type = item->GetType();
		if (type != equipType || item->IsEquipped())
			continue;
		if (type == Equipment::EquipType::ARMOUR && item->GetArmourProperties().armourType != armour_type)
			continue;

		selectedEquipment.push_back(item);
	}

	m_EquipmentSelector.SetData(selectedEquipment);
	m_Console.ClearBuffer();

}

void EquipmentMenuState::OnSlotSelect(int index, std::vector<std::wstring> data)
{
	const auto& slot_name = data[index];

	Equipment::EquipType equipType = Equipment::EquipType::NO_TYPE;
	ArmourProperties::ArmourType armour_type = ArmourProperties::ArmourType::NOT_ARMOUR;

	if (slot_name == L"Weapon")
	{
		m_eEquipSlots = Stats::EquipSlots::WEAPON;
		equipType = Equipment::EquipType::WEAPON;
	}
	else if (slot_name == L"Headgear")
	{
		m_eEquipSlots = Stats::EquipSlots::HEADGEAR;
		equipType = Equipment::EquipType::ARMOUR;
		armour_type = ArmourProperties::ArmourType::HEADGEAR;
	}
	else if (slot_name == L"Armour")
	{
		m_eEquipSlots = Stats::EquipSlots::CHEST_BODY;
		equipType = Equipment::EquipType::ARMOUR;
		armour_type = ArmourProperties::ArmourType::CHEST_BODY;
	}
	else if (slot_name == L"Footwear")
	{
		m_eEquipSlots = Stats::EquipSlots::FOOTWEAR;
		equipType = Equipment::EquipType::ARMOUR;
		armour_type = ArmourProperties::ArmourType::FOOTWEAR;
	}
	else if (slot_name == L"Accessories")
	{
		m_eEquipSlots = Stats::EquipSlots::ACCESSORY;
		equipType = Equipment::EquipType::ARMOUR;
	}
	else
	{
		assert(false, &"SHOULD BE A VALID EQUIP TYPE");
		return;
	}

	if (m_bRemoveEquipment)
	{
		RemoveEquipment(index, data);
		return;
	}

	// Filter for the armour
	std::vector<std::shared_ptr<Equipment>> selectedEquipment{};

	for (const auto& item : m_Player.GetInventory().GetEquipment())
	{
		const auto& type = item->GetType();
		if (type != equipType || item->IsEquipped())
			continue;
		if (type == Equipment::EquipType::ARMOUR && item->GetArmourProperties().armourType != armour_type)
			continue;

		selectedEquipment.push_back(item);
	}

	m_EquipmentSelector.SetData(selectedEquipment);

	m_DiffPosY = STAT_LABEL_START_Y_POS + index;
	m_sCurrentSlot = slot_name;
	m_EquipSlotSelector.HideCursor();
	//m_EquipSlotSelector.ShowCursor(); // testing
	m_EquipmentSelector.ShowCursor(); // the selector that shows up once selected weapons/headgear/armour/footwear/accessory
	m_bInSlotSelect = false;
}

void EquipmentMenuState::RenderEquip(int x, int y, std::shared_ptr<Equipment> item)
{

	if (item->IsEquipped())
		return;

	switch (item->GetType())
	{
	case Equipment::EquipType::ARMOUR:
		if (item->GetArmourProperties().armourType == ArmourProperties::ArmourType::HEADGEAR && m_eEquipSlots != Stats::EquipSlots::HEADGEAR ||
			item->GetArmourProperties().armourType == ArmourProperties::ArmourType::CHEST_BODY && m_eEquipSlots != Stats::EquipSlots::CHEST_BODY ||
			item->GetArmourProperties().armourType == ArmourProperties::ArmourType::FOOTWEAR && m_eEquipSlots != Stats::EquipSlots::FOOTWEAR
			)
			return;
		break;
	case Equipment::EquipType::WEAPON:
		if (m_eEquipSlots != Stats::EquipSlots::WEAPON)
			return;
		break;
	case Equipment::EquipType::ACCESSORY:
		if (m_eEquipSlots != Stats::EquipSlots::ACCESSORY)
			return;
		break;
	case Equipment::EquipType::NO_TYPE:
		CRPG_LOG("THERE SHOULD NEVER BE NO TYPE FOR EQUIPMENT");
		return;
	}

	const auto& name = item->GetName();
	m_Console.Write(x, y, name);
	//m_Console.Write(x, y+14, name); // Where the name of the equipment is placed on buffer once selected equip/remove, etc


	const auto& index = m_EquipmentSelector.GetIndex();
	if (index < m_EquipmentSelector.GetData().size())
	{
		const auto& item_desc = m_EquipmentSelector.GetData()[index]->GetDescription();
		m_Console.Write(m_CenterScreenW - (item_desc.size() / 2), 32, item_desc); // TODO: Center the description based on the half size and center - Item Description
		
	}
}

void EquipmentMenuState::RenderEquipSlots(int x, int y, const std::wstring& item)
{
	if (item.empty())
		return;

	// Render/Write the Slot Label Name (i.e., 'Weapon:', 'Headgear:', etc.
	m_Console.Write(x, y, item + L":"); // X = 30, Y = 15 - X moves left and right
	std::wstring equippedItem = L"";

	if (item == L"Weapon")
	{
		const auto& weapon = m_Player.GetEquippedItemSlots()[Stats::EquipSlots::WEAPON];
		if (weapon)
			equippedItem = weapon->GetName();
	}
	else if (item == L"Armour")
	{
		const auto& armour = m_Player.GetEquippedItemSlots()[Stats::EquipSlots::CHEST_BODY];
		if (armour)
			equippedItem = armour->GetName();
	}
	else if (item == L"Headgear")
	{
		const auto& head_gear = m_Player.GetEquippedItemSlots()[Stats::EquipSlots::HEADGEAR];

		if (head_gear)
			equippedItem = head_gear->GetName();
	}
	else if (item == L"Footwear")
	{
		const auto& accessory = m_Player.GetEquippedItemSlots()[Stats::EquipSlots::FOOTWEAR];

		if (accessory)
			equippedItem = accessory->GetName();
	}
	else if (item == L"Accessory")
	{
		const auto& accessory = m_Player.GetEquippedItemSlots()[Stats::EquipSlots::ACCESSORY];

		if (accessory)
			equippedItem = accessory->GetName();
	}

	if (equippedItem.empty())
		equippedItem = L"Empty";

	//std::wcout << "x: [" << x << "], x+25: [" << x + 25 << "]" << std::endl;
	m_Console.Write(x + 25, y, equippedItem);
}

void EquipmentMenuState::RemoveEquipment(int index, std::vector<std::wstring>& data)
{

	// Getting item from player
	const auto& item = m_Player.GetEquippedItemSlots()[m_eEquipSlots];

	if (!item)
		return;

	// OnRemove puts item back into the inventory & updates player stats
	item->OnRemove(m_Player);


	m_Player.GetEquippedItemSlots()[m_eEquipSlots] = nullptr;
	m_Console.ClearBuffer();
}

void EquipmentMenuState::UpdateIndex()
{
	auto index = m_EquipmentSelector.GetIndex();



	//std::cout << "Current Index: " << index << std::endl;
	//std::cout << "PrevIndex: " << m_PrevIndex << std::endl;


	// Clear the description area
	if (m_PrevIndex != index)
	{
		m_PrevIndex = index;
		m_Console.ClearBuffer();
	}
}

//void EquipmentMenuState::FocusOnMenu()
//{
//}
