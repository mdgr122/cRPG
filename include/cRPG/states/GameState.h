#pragma once
#include <memory>
#include "InterfaceState.h"
#include "../Party.h"
#include "../Selector.h"
#include "../utilities/Timer.h"
#include "../utilities/Typewriter.h"

class Console;
class Keyboard;
class StateMachine;

class GameState : public InterfaceState
{
public:
    GameState(Console &console, Keyboard &keyboard, StateMachine &stateMachine);
    ~GameState() override;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;
    bool Exit() override;

private:
    Console &m_Console;
    Keyboard &m_Keyboard;
    StateMachine &m_StateMachine;

    Selector<> m_Selector;
    // Creating the Selector object - No type given, so it should default to the wstring -- just for testing purposes
    std::unique_ptr<Party> m_Party;
    Timer m_Timer;

    //std::unique_ptr<Player> m_TestPlayer;
    //Inventory m_TestInventory;

    // Testing Typewriter
    TypeWriter m_TypeWriter;
};
