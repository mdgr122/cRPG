#pragma once

#include "Colors.h"
#include <Windows.h>
#include <memory>
#include <string>

class Console {
public:
	Console();
	~Console();

	void ClearBuffer();
	void Write(int x, int y, const std::wstring& text, WORD color = WHITE);
	void Draw();
	bool ShowConsoleCursor(bool show);


private:

	SHORT SCREEN_WIDTH = 128;
	SHORT SCREEN_HEIGHT = 48;
	SHORT BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

	//const int BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

	HANDLE hConsole_;
	HWND hConsoleWindow_;
	RECT ConsoleWindowRect_;

	DWORD BytesWritten_;
	std::unique_ptr<wchar_t[]> pScreen_;

	bool SetTextColor(int size, int x, int y, HANDLE handle, WORD color);
};