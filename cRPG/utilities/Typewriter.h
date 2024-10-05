#pragma once

#include <string>
#include <Windows.h>
#include <vector>
#include "Timer.h"
#include "../Colors.h"
#include "../utilities/Globals.h"


class Console;

class TypeWriter
{
public:
	TypeWriter(Console& console);
	TypeWriter(Console& console, int start_x, int start_y, const std::wstring& text, int text_wrap, int text_speed, WORD textColor = WHITE, WORD borderColor = WHITE);
	~TypeWriter();

	bool SetText(const std::wstring& text);
	inline void SetBorderColor(WORD color) { m_BorderColor = color; }

	void UpdateText();
	void Draw(bool showborder = true);
	inline const bool IsFinished() const { return m_bFinished; }

private:
	Console& m_Console;
	//CRPG_Globals& m_Globals;
	std::wstring m_sText, m_sCurrentText;
	int m_X, m_Y, m_BorderX, m_BorderY, m_BorderWidth, m_BorderHeight, m_TextSpeed, m_TextWrap, m_Index, m_CharIndex, m_TextIndex;
	WORD m_TextColor, m_BorderColor;
	Timer m_Timer;

	bool m_bFinished;

	std::vector<std::wstring> m_sTextChunks;	// Each line in a paragraph essentially is an element of this vector

	bool SetBorderProperties();
	void DrawBorder();
	void ClearArea();
};