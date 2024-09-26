#pragma once

#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <type_traits>
#include "Console.h"
#include "Keyboard.h"
#include "Logger.h"


// Using type_traits because we want to check if the type of the template is a w_str, if not then we don't want to write unless
// they have overriden the draw function


struct SelectorParams
{
	int x, y, columns, currentX, currentY, spacingX, spacingY;

	std::wstring cursor;

	// x and y coords -- x=20, y=10 means 20 in and 10 down in terminal
	SelectorParams(
		int x = 20, int y = 10, int columns = 1, int current_x = 0, int current_y = 0,
		int spacing_x = 20, int spacing_y = 5, std::wstring cursor = L"->"
	)
	{
		this->x = x;
		this->y = y;
		this->columns = columns;
		this->currentX = current_x;
		this->currentY = current_y;
		this->spacingX = spacing_x;
		this->spacingY = spacing_y;
		this->cursor = cursor;
	}
};

// Selector Class will be a template class, because we are going to have different types to check, which means some functions will have to
// be overridden.
// The reason for a template is because the data may change, and the cursor may not always be a wstring.
// Even though we are using a wstring when writing to the console, we still want to be able to change and manipulate the data, so
// some functions will have to be overriden by us, so we can use that data because not all data has an output string operator, so
// we will have to create out own function to write to the console


template <typename T = std::wstring>
class Selector
{
public:

	Selector();
	~Selector();

	void SetData(std::vector<T> data) { mData = data; }
	const std::vector<T>& GetData() { return mData; }
	void SetSelectionFunc(std::function<void(int, std::vector<T>)> on_selection) { m_onSelection = on_selection; }
	void SetDrawFunc(std::function<void(int, int, T)> on_draw_item) { m_onDrawItem = on_draw_item; }

	void ShowCursor() { m_bShowCursor = true; }
	void HideCursor() { m_bShowCursor = false; }

	const int GetIndex() const { return m_Params.currentX + (m_Params.currentY * m_Params.columns); } //  Grabs the current index

	// Must process inputs for keyboard events
	void ProcessInputs();
	void MoveUp(); // Moves cursor up, down, left, right
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void OnAction();

	void DrawItem(int x, int y, T item); // Our default if we do not provide a new function
	void OnSelection(int index, std::vector<T> data); // Default selection

	void Draw();

private:
	Console& m_Console;
	Keyboard& m_Keyboard;


	// std::function is a template class that can encapsulate any callable object matching a specific function signature. 
	// This flexibility makes it an essential tool for scenarios where you need to pass functions as parameters, store them in containers, or manage callbacks.
	// Function Signature: Specifies the return type and the types of its params
	// Example: int add(int, int), its function signature is int(int, int)

	//void is the return type, and int and std::vector<T> are the return params. Int will be the index of the data
	std::function<void(int, std::vector<T>)> m_OnSelection;

	// Selecting the data which we will draw - void(x, y, data)
	std::function<void(int, int, T)> m_onDrawItem;
	std::vector<T> m_data;
	SelectorParams m_Params;
	bool m_bShowCursor; //Hide or Show cursor when needed
	int m_Rows; // Determined by the number of cols and size of data
};