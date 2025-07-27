#pragma once
#include "Button.h"
#include "Keys.h"

class Keyboard
{
public:
    Keyboard();
    ~Keyboard() = default;

    void Update();
    void OnKeyDown(int key);
    void OnKeyUp(int key);

    bool IsKeyHeld(int key) const;
    bool IsKeyPressed(int key) const;
    bool IsKeyReleased(int ke) const;

private:
    Button m_Keys[KEY_LAST];
};
