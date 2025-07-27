#pragma once
#include <memory>
#include <stack>
#include "InterfaceState.h"

typedef std::unique_ptr<InterfaceState> StatePtr;

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void PushState(StatePtr newState);
    StatePtr PopState();
    const bool Empty() const { return m_States.empty(); }
    StatePtr &GetCurrentState();

private:
    std::stack<StatePtr> m_States;
};
