#include "utilities/Globals.h"
#include "utilities/crpg_utils.h"

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

std::pair<std::vector<wchar_t>, std::vector<wchar_t>> CRPG_Globals::wstr_split_to_wchar(const std::wstring& text, size_t text_pos)
{
	if (text_pos > text.length())
	{
		return { std::vector<wchar_t>(text.begin(), text.end()), {} };
	}

	std::vector<wchar_t> first_part(text.begin(), text.begin() + text_pos);
	std::vector<wchar_t> second_part(text.begin() + text_pos, text.end());

	return { first_part, second_part };
}

std::pair<std::wstring, std::wstring> CRPG_Globals::wstr_split_to_wstr(const std::wstring text, size_t text_pos)
{
	if (text_pos > text.length())
	{
		return { text, L"" };
	}
	std::wstring first_str = text.substr(0, text_pos);
	std::wstring sec_str = text.substr(text_pos);

	return { first_str, sec_str };
	//return std::pair<std::wstring, std::wstring>();
}

