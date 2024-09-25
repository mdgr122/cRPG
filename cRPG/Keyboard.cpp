#pragma once
#include "Keyboard.h"
#include "Logger.h"

Keyboard::Keyboard()
{

}

void Keyboard::Update()
{
	for (size_t i = 0; i < KEY_LAST; i++)
	{
		m_Keys[i].m_bIsPressed = false;
		m_Keys[i].m_bIsReleased = false;
	}
}

void Keyboard::OnKeyDown(int key)
{
	// Check to see if key is defined in virtual key list
	if (key > KEY_LAST)
	{
		CRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return;
	}
	return m_Keys[key].Update(true);
}

void Keyboard::OnKeyUp(int key)
{
	// Check to see if key is defined in virtual key list
	if (key > KEY_LAST)
	{
		CRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return;
	}
	return m_Keys[key].Update(false);
}

bool Keyboard::IsKeyHeld(int key) const
{
	// Check to see if key is defined in virtual key list
	if (key > KEY_LAST)
	{
		CRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return m_Keys[key].m_bIsDown;
}

bool Keyboard::IsKeyPressed(int key) const
{
	// Check to see if key is defined in virtual key list
	if (key > KEY_LAST)
	{
		CRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return m_Keys[key].m_bIsPressed;
}

bool Keyboard::IsKeyReleased(int key) const
{
	// Check to see if key is defined in virtual key list
	if (key > KEY_LAST)
	{
		CRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return m_Keys[key].m_bIsReleased;
}