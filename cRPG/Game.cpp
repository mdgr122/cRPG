#include "Game.h"
#include "Logger.h"
#include <iostream>

bool Game::Init()
{
	try 
	{
		pConsole_ = std::make_unique<Console>();
	}
	catch (std::exception& e)
	{
		std::string error = e.what();
		CRPG_ERROR(error);
		return false;
	}

	hConsoleIn_ = GetStdHandle(STD_INPUT_HANDLE);

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
}

void Game::Update()
{
}

void Game::Draw()
{
	pConsole_->Write(10, 10, L"Hello World!", RED);
	pConsole_->Draw();
}

void Game::KeyEventProcess(KEY_EVENT_RECORD key_event)
{
	if (key_event.bKeyDown)
	{
		// TODO: Keyboard class will go here.
		std::cout << "Key Pressed: " << key_event.wVirtualKeyCode << std::endl;
	}
	else
	{
		std::cout << "Key Released: " << key_event.wVirtualKeyCode << std::endl;

	}
}


Game::Game() : is_running_{true}
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
		ProcessInputs();
		ProcessEvents();
		Update();
		Draw();
	}
	CRPG_LOG("Terminating Program\n")
}

