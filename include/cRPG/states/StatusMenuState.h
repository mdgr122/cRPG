#pragma once
#include "InterfaceState.h"
#include "../Equipment.h"
#include "../Selector.h"

class Party;
class Console;
class StateMachine;
class Keyboard;
class Player;

class StatusMenuState : public InterfaceState
{
public:
    StatusMenuState(Player &player, Console &console, StateMachine &stateMachine, Keyboard &keyboard);
    ~StatusMenuState() override;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

    bool Exit() override;

private:
    const int PANEL_BARS             = 90;
    const int STATUS_SIZE            = 31;
    const int STAT_VAL_X_POS         = 70;
    const int STAT_LABEL_X_POS       = 50;
    const int STAT_LABEL_START_Y_POS = 30;


    Console &m_Console;
    StateMachine &m_StateMachine;
    Keyboard &m_Keyboard;
    Player &m_Player;

    int m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX, m_DiffPosY;


    void DrawStatusPanel();
    void DrawPlayerInfo();
};
