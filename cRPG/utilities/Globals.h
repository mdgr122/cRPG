#pragma once
#include <memory>
#include <string>
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

private:
	int m_GameTime, m_SavedGameTime;
	Timer m_Timer;

	CRPG_Globals();

	// Singleton because we only want 1 instance of globals.
	static std::unique_ptr<CRPG_Globals> m_pInstance;
};