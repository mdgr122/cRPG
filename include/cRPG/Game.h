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
    bool m_bIsRunning;

    std::unique_ptr<Console> m_pConsole;
    std::unique_ptr<Keyboard> m_pKeyboard;
    std::unique_ptr<StateMachine> m_pStateMachine;

    DWORD m_NumRead;
    INPUT_RECORD m_InRecBuf[128];
    HANDLE m_hConsoleIn;

    bool Init();

    void ProcessInputs();
    void ProcessEvents();
    void Update();
    void Draw();

    void KeyEventProcess(KEY_EVENT_RECORD key_event);
};

#endif // !GAME_H
