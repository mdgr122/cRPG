#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Timer.h"

class CRPG_Globals
{
public:
	// Singleton because we only want 1 instance of globals.
	static CRPG_Globals& GetInstance();

	const int GetGameTime() const { return m_GameTime; }
	void SetSaveGameTime(int saved_time) { m_SavedGameTime = saved_time; } // Reading in game time from a file
	void Update();

	const std::wstring GetTime(); // Not the same as GetGameTime().

	std::pair<std::wstring, std::wstring> wstr_split_to_wstr(const std::wstring text, size_t  text_pos);
	std::pair<std::vector<wchar_t>, std::vector<wchar_t>> wstr_split_to_wchar(const std::wstring& text, size_t text_pos);

private:
	int m_GameTime, m_SavedGameTime;
	Timer m_Timer;

	CRPG_Globals();

	// Singleton because we only want 1 instance of globals.
	static std::unique_ptr<CRPG_Globals> m_pInstance;
};