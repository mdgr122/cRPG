#include "GameState.h"
#include "StateMachine.h"
#include "../Logger.h"
#include "../Console.h"
#include "../Keyboard.h"
#include "../Potion.h"
#include "../utilities/ItemCreator.h"
#include "GameMenuState.h"
#include "../utilities/ItemLoader.h"
#include "../utilities/EquipmentLoader.h"
#include <cassert>



GameState::GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine) 
	: m_Console(console)
	, m_Keyboard(keyboard)
	, m_StateMachine(stateMachine)
	, m_Selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
	, m_Party{nullptr}
	, m_Timer{}
	//, m_TestInventory{} // TO BE REMOVED
{

	m_Party = std::make_unique<Party>();

	// Create an item to add to inventory test
	auto potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Potion", L"Heals for a small amount", 25, 50);
	auto strong_potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Strong Potion", L"Heals a large amount of health", 250, 300);
	auto sword = ItemCreator::CreateEquipment(Equipment::EquipType::WEAPON, 
		WeaponProperties(15, WeaponProperties::WeaponType::SWORD),
		ArmourProperties(),
		StatModifier(3, StatModifier::ModifierType::STRENGTH),
		L"Iron Sword",
		L"A standard iron sword",
		100, 50);

	auto helmet = ItemCreator::CreateEquipment(Equipment::EquipType::ARMOUR,
		WeaponProperties(),
		ArmourProperties(30, ArmourProperties::ArmourType::HEADGEAR),
		StatModifier(),
		L"Iron Helmet",
		L"A battered iron helmet",
		45, 15);


	m_Party->GetInventory().AddItem(std::move(potion));
	m_Party->GetInventory().AddItem(std::move(strong_potion));
	m_Party->GetInventory().AddEquipment(std::move(sword));
	m_Party->GetInventory().AddEquipment(std::move(helmet));

	
	auto player = std::make_shared<Player>(L"Test PLayer", L"1", m_Party->GetInventory(), 1, 100);
	//auto player2 = std::make_shared<Player>(L"Player2", L"2", m_Party->GetInventory(), 1, 100);
	m_Party->AddMember(std::move(player));
	//m_Party->AddMember(std::move(player2));

}

GameState::~GameState()
{
}

void GameState::OnEnter()
{
	m_Console.ClearBuffer();
	ItemLoader il{ "C:/Users/MDaki/source/repos/cRPG/cRPG/Assets/xml_files/ItemDefs.xml" };
	auto item = il.CreateObjectFromFile("Strong Potion");

	EquipmentLoader el{ "C:/Users/MDaki/source/repos/cRPG/cRPG/Assets/xml_files/WeaponDefs.xml" };
	auto equipment = el.CreateObjectFromFile("Iron Sword");
	assert(equipment);
}

void GameState::OnExit()
{
	m_Console.ClearBuffer();
}

void GameState::Update()
{
	// Not sure if correctly placed
	//m_Console.ClearBuffer();
}

void GameState::Draw()
{

	
	//const auto& stat_labels = m_TestPlayer->GetStats().GetStatList();

	for (const auto& member : m_Party->GetParty())
	{
		const auto& name = member->GetName();
		std::wstring hp = std::to_wstring(member->GetHP());
		std::wstring max_hp = std::to_wstring(member->GetMaxHP());

		m_Console.Write(50, 30, name, BLUE);
		m_Console.Write(50, 32, L"HP: " + hp + L"/" + max_hp, BLUE);
		
		const auto& stats_list = member->GetStats().GetStatList();
		int i = 0;
		for (const auto& [stat, value] : stats_list)
		{
			const auto& mod_value = member->GetStats().GetModifier(stat);
			m_Console.Write(50, 34 + i, stat + L":");
			m_Console.Write(70, 34 + i, std::to_wstring(value + mod_value));
			i++;
		}
	}


	//m_Console.Write(55, 40, stats, RED);

	std::wstring time_seconds = std::to_wstring(m_Timer.ElapsedSec());
	std::wstring time_ms = std::to_wstring(m_Timer.ElapsedMS());
	m_Console.Write(25, 26, time_ms, BLUE);
	m_Console.Write(25, 25, time_seconds, RED);
	
	// This is where the initialization of the GameState::GameState object is drawn 
	m_Selector.Draw(); // m_Selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
	m_Console.Draw();
}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
		return;
	}

	if (m_Keyboard.IsKeyPressed(KEY_M))
	{
		m_StateMachine.PushState(std::make_unique<GameMenuState>(*m_Party, m_Console, m_StateMachine, m_Keyboard));
		return;
	}

	if (m_Keyboard.IsKeyPressed(KEY_ENTER))
	{
		m_Timer.Start();
	}
	else if (m_Keyboard.IsKeyPressed(KEY_P))
	{
		m_Timer.Pause();
	}
	else if (m_Keyboard.IsKeyPressed(KEY_R))
	{
		m_Timer.Resume();
	}
	else if (m_Keyboard.IsKeyPressed(KEY_T))
	{
		m_Timer.Stop();
		m_Console.ClearBuffer();
	}

	m_Selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
