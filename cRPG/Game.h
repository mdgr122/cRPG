#ifndef GAME_H
#define GAME_H
#include "Console.h"
class Game
{
public:
	Game();
	~Game();

	void Run();

private:

	bool is_running_;

	std::unique_ptr<Console> pConsole_;

	DWORD NumRead_;
	INPUT_RECORD InRecBuff_[128];
	HANDLE hConsoleIn_;

	bool Init();

	void ProcessEvents();
	void ProcessInputs();
	void Update();
	void Draw();

	void KeyEventProcess(KEY_EVENT_RECORD key_event);
};

#endif // !GAME_H
