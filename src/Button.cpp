#include "Button.h"


void Button::Update(bool pressed)
{

	// if key is down AND 
	m_bIsPressed = !m_bIsDown && pressed;
	m_bIsReleased = m_bIsDown && !pressed;
	m_bIsDown = pressed;
}