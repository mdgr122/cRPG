#pragma once
#include "InterfaceState.h"
#include "../Equipment.h"
#include "../Selector.h"
#include "../Stats.h"

class Party;
class Console;
class StateMachine;
class Keyboard;
class Player;

class EquipmentMenuState : public InterfaceState
{
public:
    EquipmentMenuState(Player &player, Console &console, StateMachine &stateMachine, Keyboard &keyboard);
    ~EquipmentMenuState() override;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

    bool Exit() override;

private:
    const int PANEL_BARS             = 90;
    const int EQUIP_SIZE             = 52;
    const int STAT_PREDICT_X_POS     = 75;
    const int STAT_VAL_X_POS         = 50;
    const int STAT_LABEL_X_POS       = 30;
    const int STAT_LABEL_START_Y_POS = 23;


    Console &m_Console;
    StateMachine &m_StateMachine;
    Keyboard &m_Keyboard;
    Player &m_Player;

    Selector<> m_MenuSelector;
    Selector<> m_EquipSlotSelector;
    Selector<std::shared_ptr<Equipment>> m_EquipmentSelector;

    bool m_bExitGame, m_bInMenuSelect, m_bInSlotSelect, m_bRemoveEquipment;
    int m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX, m_DiffPosY, m_PrevStatModPos, m_PrevIndex;

    std::wstring m_sCurrentSlot;
    Stats::EquipSlots m_eEquipSlots;

    void DrawEquipment();
    void DrawPlayerInfo();
    void DrawStatPrediction();
    void DrawStatModifier(int x, int y, const std::wstring &stat, int value);


    void OnMenuSelect(int index, std::vector<std::wstring> data);
    // What menu are we selecting
    void OnEquipSelect(int index, std::vector<std::shared_ptr<Equipment>> data);
    // Vector of shared pointers of items -- Wil include the inventory of items
    void OnSlotSelect(int index, std::vector<std::wstring> data);
    void RenderEquip(int x, int y, std::shared_ptr<Equipment> item);
    // draw item checks to see if wstring, if not then we need to override.
    void RenderEquipSlots(int x, int y, const std::wstring &item);

    void RemoveEquipment(int index, std::vector<std::wstring> &data);
    void UpdateIndex();
    //void FocusOnMenu();
};
