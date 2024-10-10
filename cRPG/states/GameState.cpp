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
#include "../utilities/ShopLoader.h"
#include <cassert>



GameState::GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine) 
	: m_Console(console)
	, m_Keyboard(keyboard)
	, m_StateMachine(stateMachine)
	, m_Selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
	, m_Party{nullptr}
	, m_Timer{}
	, m_TypeWriter{console, 50, 4, 
	L"The adventure began at the crack of dawn. Travelers from distant lands gathered near the ancient castle. The procession of carriages and horses stretched for miles, with every person eager to reach their final destination. As the sun rose higher, the landscape slowly transformed. Mountains loomed in the distance, casting long shadows over the vast plains. Many whispered tales of hidden treasures buried deep within the mountains. Some spoke of dangers lurking within, while others were drawn to the unknown mysteries that awaited. The road ahead was long, but their determination never wavered."
	, 60, 5, WHITE, BLUE}
	//, m_TestInventory{} // TO BE REMOVED
{

	m_Party = std::make_unique<Party>();

	// Create an item to add to inventory test
	auto potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Potion", L"Heals for a small amount", 25, 50);
	auto strong_potion = ItemCreator::CreateItem(Item::ItemType::HEALTH, L"Strong Potion", L"Heals a large amount of health", 250, 300);
	
	auto sword = ItemCreator::CreateEquipment(Equipment::EquipType::WEAPON, 
		WeaponProperties(50, WeaponProperties::WeaponType::SWORD),
		ArmourProperties(),
		StatModifier(3, StatModifier::ModifierType::STRENGTH),
		L"Iron Sword",
		L"A standard iron sword",
		100, 50);

	auto helmet = ItemCreator::CreateEquipment(Equipment::EquipType::ARMOUR,
		WeaponProperties(),
		ArmourProperties(30, ArmourProperties::ArmourType::HEADGEAR),
		StatModifier(28, StatModifier::ModifierType::STRENGTH),
		L"Iron Helmet",
		L"A battered iron helmet",
		45, 15);

	auto chest = ItemCreator::CreateEquipment(Equipment::EquipType::ARMOUR,
		WeaponProperties(),
		ArmourProperties(45, ArmourProperties::ArmourType::CHEST_BODY),
		StatModifier(),
		L"Iron Chest",
		L"An Iron Chestplate",
		300, 185);

	m_Party->GetInventory().AddItem(std::move(potion));
	m_Party->GetInventory().AddItem(std::move(strong_potion));
	m_Party->GetInventory().AddEquipment(std::move(sword));
	m_Party->GetInventory().AddEquipment(std::move(helmet));
	m_Party->GetInventory().AddEquipment(std::move(chest));

	
	auto Player1 = std::make_shared<Player>(L"Player 1", L"1", m_Party->GetInventory(), 1, 100, 20);
	auto Player2 = std::make_shared<Player>(L"Player 2", L"2", m_Party->GetInventory(), 1, 100, 20);
	auto Player3 = std::make_shared<Player>(L"Player 3", L"3", m_Party->GetInventory(), 1, 100, 20);
	//auto player2 = std::make_shared<Player>(L"Player2", L"2", m_Party->GetInventory(), 1, 100);
	m_Party->AddMember(std::move(Player1));
	m_Party->AddMember(std::move(Player2));
	m_Party->AddMember(std::move(Player3));
	
	ShopLoader shop_loader{};
	auto shopParameters = shop_loader.CreateShopParametersFromFile("C:/Users/MDaki/source/repos/cRPG/cRPG/Assets/xml_files/ArmourShopDefs_1.xml");
	assert(shopParameters, &"Failed to create shop params");

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
	m_TypeWriter.UpdateText();
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

	m_TypeWriter.Draw();

	m_Console.Draw();
}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
		return;
	}

	//if (m_Keyboard.IsKeyPressed(KEY_M))
	if (m_Keyboard.IsKeyPressed(KEY_SPACE))
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
