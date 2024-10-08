#pragma once

#include <Windows.h>
#include "Colors.h"
#include <memory>
#include <string>
class Console {
public:
	Console();
	~Console();

	const int GetScreenWidth() const { return SCREEN_WIDTH; }
	const int GetScreenHeight() const { return SCREEN_HEIGHT; }
	const int GetHalfWidth() const { return HALF_WIDTH; }
	const int GetHalfHeight() const { return HALF_HEIGHT; }

	void ClearBuffer();
	void Write(int x, int y, const std::wstring& text, WORD color = WHITE);
	void Draw();
	bool ShowConsoleCursor(bool show);

	void DrawPanelHorz(int x, int y, size_t length, WORD color = WHITE, const std::wstring& character = L"=");
	void DrawPanelVert(int x, int y, size_t height, WORD color = WHITE, const std::wstring& character = L"|");
	void DrawPanel(
		int x, int y, size_t width, size_t height, WORD color = WHITE,
		const std::wstring& width_char = L"=",
		const std::wstring& height_char = L"|"
		//const std::wstring& width_char = L"+",
		//const std::wstring& height_char = L"+"
	);




private:

	SHORT SCREEN_WIDTH = 128;
	SHORT SCREEN_HEIGHT = 48;
	SHORT BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
	SHORT HALF_WIDTH = SCREEN_WIDTH / 2;
	SHORT HALF_HEIGHT = SCREEN_HEIGHT / 2;


	HANDLE hConsole_;
	HWND hConsoleWindow_;
	RECT ConsoleWindowRect_;

	DWORD BytesWritten_;

	std::unique_ptr<wchar_t[]> pScreen_;

	bool SetTextColor(int size, int x, int y, HANDLE handle, WORD color);
	void DrawBorder();
};