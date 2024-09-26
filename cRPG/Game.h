#ifndef GAME_H
#define GAME_H
#include "Console.h"
#include "Keyboard.h"
#include "states/StateMachine.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:

	bool is_running_;

	std::unique_ptr<Console> pConsole_;
	std::unique_ptr<Keyboard> pKeyboard_;
	std::unique_ptr<StateMachine> pStateMachine_;

	DWORD NumRead_;
	INPUT_RECORD InRecBuff_[128];
	HANDLE hConsoleIn_;

	bool Init();

	void ProcessInputs();
	void ProcessEvents();
	void Update();
	void Draw();

	void KeyEventProcess(KEY_EVENT_RECORD key_event);
};

#endif // !GAME_H
