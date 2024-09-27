#include "Game.h"
#include "Logger.h"
#include <iostream>
#include "states/GameState.h"

bool Game::Init()
{
	try 
	{
		// Creating a unique_ptr managing a new Console instance
		pConsole_ = std::make_unique<Console>();
	}
	catch (std::exception& e)
	{
		std::string error = e.what();
		CRPG_ERROR(error);
		return false;
	}

	hConsoleIn_ = GetStdHandle(STD_INPUT_HANDLE);
	
	// Creating a unique_ptr managing a new Keyboard instance
	// std::make_unique<Keyboard>() creates a new Keyboard object on the heap, and returns a std::unique_ptr<Keyboard> managing it. 
	// The returned std::unique_ptr<Keyboard> is assigned to pKeyboard_, which now exclusively owns the Keyboard object. 
	// When pKeyboard_ goes out of scope, the object is automatically deleted.
	pKeyboard_ = std::make_unique<Keyboard>();
	pStateMachine_ = std::make_unique<StateMachine>();

	pStateMachine_->PushState(std::make_unique<GameState>(*pConsole_, *pKeyboard_, *pStateMachine_));

	return true;
}

void Game::ProcessEvents()
{
	// Get number of Console Inputs
	if (!GetNumberOfConsoleInputEvents(hConsoleIn_, &NumRead_))
	{
		DWORD error = GetLastError();
		CRPG_ERROR("Failed to get number of console input events! " + error);
		return;
	}

	if (NumRead_ <= 0)
		return;


	// PeekConsoleInput doesn't remove any input from the buffer, compared to ReadConsoleInput, which does.
	if (!PeekConsoleInput(hConsoleIn_, InRecBuff_, 128, &NumRead_))
	{
		DWORD error = GetLastError();
		CRPG_ERROR("Failed to peek console input! " + error);
		return;
	}

	for (int i = 0; i < NumRead_; i++)
	{
		switch (InRecBuff_[i].EventType)
		{
		case KEY_EVENT:
			KeyEventProcess(InRecBuff_[i].Event.KeyEvent);
			break;
		default:
			break;
		}
	}

	// Flush Other Inputs because PeekConsoleInput doesn't remove them from the buffer.
	FlushConsoleInputBuffer(hConsoleIn_);
}

void Game::ProcessInputs()
{
	if (pKeyboard_->IsKeyPressed(KEY_ESCAPE))
	{
		is_running_ = false;
	}
	if (pStateMachine_->Empty())
	{
		CRPG_ERROR("No States exist in the StateMachine to Process Inputs");
		is_running_ = false;
		return;
	}
	pStateMachine_->GetCurrentState()->ProcessInputs();

}

void Game::Update()
{

	if (pStateMachine_->Empty())
	{
		CRPG_ERROR("No States exist in the StateMachine to Update");
		is_running_ = false;
		return;
	}

	pStateMachine_->GetCurrentState()->Update();
	pKeyboard_->Update();
}

void Game::Draw()
{
	if (pStateMachine_->Empty())
	{
		CRPG_ERROR("No States exist in the StateMachine to Draw");
		is_running_ = false;
		return;
	}

	pStateMachine_->GetCurrentState()->Draw();
	//pConsole_->Write(10, 10, L"Hello World!", RED);
	pConsole_->Draw();
}

void Game::KeyEventProcess(KEY_EVENT_RECORD key_event)
{
	if (key_event.bKeyDown)
	{
		pKeyboard_->OnKeyDown(key_event.wVirtualKeyCode);
	}
	else
	{
		pKeyboard_->OnKeyUp(key_event.wVirtualKeyCode);

	}
}


Game::Game() : is_running_{true}, pConsole_{nullptr}, pKeyboard_{nullptr}, pStateMachine_{nullptr}
{

}

Game::~Game()
{

}

void Game::Run()
{
	if (!Init())
		is_running_ = false;
	
	while (is_running_) 
	{
		ProcessEvents();
		ProcessInputs();
		Update();
		Draw();
	}
	CRPG_LOG("Exiting Program\n")
}

