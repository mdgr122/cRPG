#include "Globals.h"
#include "crpg_utils.h"

std::unique_ptr<CRPG_Globals> CRPG_Globals::m_pInstance = nullptr;

CRPG_Globals::CRPG_Globals()
	: m_GameTime{ 0 }
	, m_SavedGameTime{ 0 }
	, m_Timer{}
{
	m_Timer.Start();
}

CRPG_Globals& CRPG_Globals::GetInstance()
{
	if (!m_pInstance)
		m_pInstance.reset(new CRPG_Globals());

	return *m_pInstance;
}

void CRPG_Globals::Update()
{
	m_GameTime = m_Timer.ElapsedSec() + m_SavedGameTime;
}

const std::wstring CRPG_Globals::GetTime()
{
	int hours = m_GameTime / 3600;
	int minutes = (m_GameTime % 3600) / 60;
	int seconds = m_GameTime % 60;

	std::wstring time{ PadNumbers(hours) + L":" + PadNumbers(minutes) + L":" + PadNumbers(seconds) };
	
	return time;
}

