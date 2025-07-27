#include "Game.h"
#include "Logger.h"
#include <iostream>
#include "states/GameState.h"
#include "utilities/Globals.h"

bool Game::Init()
{
	try 
	{
		// Creating a unique_ptr managing a new Console instance
		m_pConsole = std::make_unique<Console>();
	}
	catch (std::exception& e)
	{
		std::string error = e.what();
		CRPG_ERROR(error);
		return false;
	}

	m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	
	// Creating a unique_ptr managing a new Keyboard instance
	// std::make_unique<Keyboard>() creates a new Keyboard object on the heap, and returns a std::unique_ptr<Keyboard> managing it. 
	// The returned std::unique_ptr<Keyboard> is assigned to m_pKeyboard, which now exclusively owns the Keyboard object. 
	// When m_pKeyboard goes out of scope, the object is automatically deleted.
	m_pKeyboard = std::make_unique<Keyboard>();
	m_pStateMachine = std::make_unique<StateMachine>();

	m_pStateMachine->PushState(std::make_unique<GameState>(*m_pConsole, *m_pKeyboard, *m_pStateMachine));

	return true;
}

void Game::ProcessEvents()
{
	// Get number of Console Inputs
	if (!GetNumberOfConsoleInputEvents(m_hConsoleIn, &m_NumRead))
	{
		DWORD error = GetLastError();
		CRPG_ERROR("Failed to get number of console input events! " + error);
		return;
	}

	if (m_NumRead <= 0)
		return;


	// PeekConsoleInput doesn't remove any input from the buffer, compared to ReadConsoleInput, which does.
	if (!PeekConsoleInput(m_hConsoleIn, m_InRecBuf, 128, &m_NumRead))
	{
		DWORD error = GetLastError();
		CRPG_ERROR("Failed to peek console input! " + error);
		return;
	}

	for (int i = 0; i < m_NumRead; i++)
	{
		switch (m_InRecBuf[i].EventType)
		{
		case KEY_EVENT:
			KeyEventProcess(m_InRecBuf[i].Event.KeyEvent);
			break;
		default:
			break;
		}
	}

	// Flush Other Inputs because PeekConsoleInput doesn't remove them from the buffer.
	FlushConsoleInputBuffer(m_hConsoleIn);
}

void Game::ProcessInputs()
{
	if (m_pKeyboard->IsKeyPressed(KEY_ESCAPE))
		m_bIsRunning = false;


	if (m_pStateMachine->Empty())
	{
		CRPG_ERROR("No States exist in the StateMachine to Process Inputs");
		m_bIsRunning = false;
		return;
	}
	m_pStateMachine->GetCurrentState()->ProcessInputs();

}

void Game::Update()
{

	if (m_pStateMachine->Empty())
	{
		//CRPG_ERROR("No States exist in the StateMachine to Update");
		m_bIsRunning = false;
		return;
	}

	m_pStateMachine->GetCurrentState()->Update();
	m_pKeyboard->Update();


	// Updates the gametime and calls the instance irght away. Recall the instance is a singleton, so only one instance.
	CRPG_Globals::GetInstance().Update();
}

void Game::Draw()
{
	if (m_pStateMachine->Empty())
	{
		//CRPG_ERROR("No States exist in the StateMachine to Draw");
		m_bIsRunning = false;
		return;
	}

	m_pStateMachine->GetCurrentState()->Draw();
	//m_pConsole->Write(10, 10, L"Hello World!", RED);
	m_pConsole->Draw();
}

void Game::KeyEventProcess(KEY_EVENT_RECORD key_event)
{
	if (key_event.bKeyDown)
	{
		m_pKeyboard->OnKeyDown(key_event.wVirtualKeyCode);
	}
	else
	{
		m_pKeyboard->OnKeyUp(key_event.wVirtualKeyCode);

	}
}


Game::Game() 
	: m_bIsRunning{true}
	, m_pConsole{nullptr}
	, m_pKeyboard{nullptr}
	, m_pStateMachine{nullptr}
{
}

Game::~Game()
{
}

void Game::Run()
{
	if (!Init())
		m_bIsRunning = false;
	
	while (m_bIsRunning) 
	{
		ProcessEvents();
		ProcessInputs();
		Update();
		Draw();
	}
	CRPG_LOG("Exiting Program\n")
}

