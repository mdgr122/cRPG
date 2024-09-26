#include "GameState.h"
#include "../Logger.h"
#include "../Keyboard.h"
#include "StateMachine.h"


GameState::GameState(Keyboard& keyboard, StateMachine& Statemachine) : m_Keyboard(keyboard), m_StateMachine(Statemachine)
{
}

GameState::~GameState()
{
}

void GameState::OnEnter()
{
	CRPG_LOG("Entered GAME STATE")
}

void GameState::OnExit()
{
	CRPG_LOG("EXITING GAME STATE")
}

void GameState::Update()
{
	CRPG_LOG("GAME STATE UPDATED!")
}

void GameState::Draw()
{
}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
	}
}

bool GameState::Exit()
{
	return false;
}
