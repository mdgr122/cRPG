#pragma once

class InterfaceState
{
public:
    virtual ~InterfaceState() {}

    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void ProcessInputs() = 0;

    virtual bool Exit() = 0;
};
