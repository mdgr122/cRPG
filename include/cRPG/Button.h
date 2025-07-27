#pragma once

struct Button
{
	bool m_bIsDown{ false };
	bool m_bIsPressed{ false };
	bool m_bIsReleased{ false };
	void Update(bool pressed);
};