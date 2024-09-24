#include "Console.h"
#include "Logger.h"
#include "Colors.h"
#include <vector>

Console::Console() : pScreen_{nullptr}
{

	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD maxSize = GetLargestConsoleWindowSize(hConsole);
	bool bAdjustBuffer{ false };
	int minX{ GetSystemMetrics(SM_CXMIN) };
	int minY{ GetSystemMetrics(SM_CYMIN) };

	if (minX > SCREEN_WIDTH)
	{
		SCREEN_WIDTH = static_cast<SHORT>(minX);
		bAdjustBuffer = true;
	}
	if (minY > SCREEN_HEIGHT)
	{
		SCREEN_HEIGHT = static_cast<SHORT>(minY);
		bAdjustBuffer = true;
	}

	// Ensure SCREEN_WIDTH and SCREEN_HEIGHT do not exceed the maximum allowed console size
	if (SCREEN_WIDTH > maxSize.X) {
		SCREEN_WIDTH = maxSize.X;
		bAdjustBuffer = true;
	}
	if (SCREEN_HEIGHT > maxSize.Y) {
		SCREEN_HEIGHT = maxSize.Y;
		bAdjustBuffer = true;
	}

	if (bAdjustBuffer)
		BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

	COORD consoleBuffer{ .X = SCREEN_WIDTH, .Y = SCREEN_HEIGHT };

	if (!SetConsoleScreenBufferSize(hConsole, consoleBuffer))
	{
		auto error = GetLastError();
		CRPG_ERROR("Error: " + std::to_string(error));
		throw("Failed to set the console screen buffer size when creating the console!");
	}

	SMALL_RECT windowRect{ .Left = 0, .Top = 0, .Right = SCREEN_WIDTH - 1, .Bottom = SCREEN_HEIGHT - 1 };

	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowRect))
	{
		auto error = GetLastError();
		CRPG_ERROR("Error: " + std::to_string(error));
		throw("Failed to set the console window info when creating the console!");
	}

	// Get a handle to a console window
	hConsoleWindow_ = GetConsoleWindow();

	if (!GetWindowRect(hConsoleWindow_, &ConsoleWindowRect_))
		throw("Failed to get window Rect when creating the console!");

	int pos_x = GetSystemMetrics(SM_CXSCREEN) / 2 - (ConsoleWindowRect_.right - ConsoleWindowRect_.left) / 2;
	int pos_y = GetSystemMetrics(SM_CYSCREEN) / 2 - (ConsoleWindowRect_.bottom - ConsoleWindowRect_.top) / 2;

	SetWindowPos(hConsoleWindow_, NULL, pos_x, pos_y, -1, -1, SWP_NOSIZE);


	// Initialize the screen buffer
	pScreen_ = std::make_unique<wchar_t[]>(BUFFER_SIZE);

	// Clear screen buffer
	ClearBuffer();

	// Create the screen buffer
	hConsole_ = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	if (!hConsole_)
		throw("Failed to create the console screen buffer");

	// Set the buffer to be active
	if (!SetConsoleActiveScreenBuffer(hConsole_))
		throw("Failed to set the active screen buffer");

	// Hide the cursor
	if (!ShowConsoleCursor(false))
		throw("Failed to hide the console cursor");

	// Get the font size
	// CONSOLE_FONT_INFO font_info;

	//if (!GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font_info))
	//	throw("Failed to get the console font!");

	//COORD font_size = GetConsoleFontSize(GetStdHandle(STD_OUTPUT_HANDLE), font_info.nFont);
	//int actual_screen_x = SCREEN_WIDTH * font_size.X;
	//int actual_screen_y = SCREEN_HEIGHT * font_size.Y;


	// Set the size and position of window
	//if (!MoveWindow(hConsoleWindow_, pos_x, pos_y, actual_screen_x, actual_screen_y, TRUE))
	//	throw("Failed to set and move the console window to center of screen");



	//SetTextColor(BUFFER_SIZE, pos_x, pos_y, hConsole_, RED);



	
}

Console::~Console()
{

}

void Console::ClearBuffer()
{
	// Set all the values of the buffer to an empty space in memory
	for (int i = 0; i < BUFFER_SIZE; i++)
		pScreen_[i] = L' ';

	// Reset buffer to white
	SetTextColor(BUFFER_SIZE, 0, 0, hConsole_, WHITE);
}


void Console::Write(int x, int y, const std::wstring& text, WORD color)
{

	SetTextColor(text.size(), x, y, hConsole_, color);

	int pos = y * SCREEN_WIDTH + x;
	//swprintf(&pScreen_[pos], BUFFER_SIZE, text.c_str());
	if (pos < BUFFER_SIZE)
		swprintf(&pScreen_[pos], BUFFER_SIZE - pos, text.c_str());  // Ensure not to overwrite beyond BUFFER_SIZE
}

void Console::Draw()
{
	// Handle all console drawings
	WriteConsoleOutputCharacterW(hConsole_, pScreen_.get(), BUFFER_SIZE, { 0, 0 }, &BytesWritten_);
}

bool Console::ShowConsoleCursor(bool show)
{
	CONSOLE_CURSOR_INFO cursor_info;
	if (!GetConsoleCursorInfo(hConsole_, &cursor_info))
	{
		CRPG_ERROR("Failed to get the cursor info");
		return false;
	}
	cursor_info.bVisible = show;
	return SetConsoleCursorInfo(hConsole_, &cursor_info);
}

bool Console::SetTextColor(int size, int x, int y, HANDLE handle, WORD color)
{
	COORD pos = { x, y };

	std::vector<WORD> write(size, color);

	// Initializing double word
	DWORD written = 0;
	LPDWORD lpNumWritten = &written;

	if (!WriteConsoleOutputAttribute(handle, &write[0], size, pos, lpNumWritten))
	{
		CRPG_ERROR("Unable to change text color!");
		return false;
	}

	return true;
}
