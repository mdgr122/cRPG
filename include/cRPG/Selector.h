#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>
#include "Console.h"
#include "Keyboard.h"
#include "Logger.h"


// Using type_traits because we want to check if the type of the template is a w_str, if not then we don't want to write unless
// they have overriden the draw function


struct SelectorParams
{
    int x, y, columns, currentX, currentY, spacingX, spacingY;

    std::wstring cursor;

    // x and y coords -- x=20, y=10 means 20 in and 10 down in terminal - Old cursor ->
    SelectorParams(
        //int x = 20, int y = 10, int columns = 1, int current_x = 0, int current_y = 0,
        //int spacing_x = 20, int spacing_y = 5, std::wstring cursor = L">"
        int x               = 20,
        int y               = 10,
        int columns         = 1,
        int current_x       = 0,
        int current_y       = 0,
        int spacing_x       = 20,
        int spacing_y       = 1,
        std::wstring cursor = L">")
    {
        this->x        = x;
        this->y        = y;
        this->columns  = columns;
        this->currentX = current_x;
        this->currentY = current_y;
        this->spacingX = spacing_x;
        this->spacingY = spacing_y;
        this->cursor   = cursor;
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
    Selector(Console &console, Keyboard &keyboard, std::vector<T> data, SelectorParams params = SelectorParams());
    Selector(Console &console,
             Keyboard &keyboard,
             std::function<void(int, std::vector<T>)> on_selection,
             std::function<void(int, int, T)> on_draw_item,
             std::vector<T> data,
             SelectorParams params = SelectorParams());

    ~Selector();

    void SetData(std::vector<T> data)
    {
        m_Data = data;

        // Initialize the number of rows - Uses a Ternary Operator (short if statement, essentially)
        // Ternary Operator: condition ? expression1 : expression2; If true, execute expression 1, if false execute expression 2 
        // If params.columns == 0 then return 1, if false then return params.columns
        // Adding this m_Rows line to the SetData void func reset or re-initializes the number of rows needed for each item in their respective shops.
        m_Rows = static_cast<int>(std::ceil(m_Data.size() / (m_Params.columns == 0 ? 1 : m_Params.columns)));
    }

    std::vector<T> &GetData() { return m_Data; }

    void SetSelectionFunc(std::function<void(int, std::vector<T>)> on_selection)
    {
        m_OnSelection = on_selection;
    }

    void SetDrawFunc(std::function<void(int, int, T)> on_draw_item)
    {
        m_OnDrawItem = on_draw_item;
    }

    void ShowCursor() { m_bShowCursor = true; }
    void HideCursor() { m_bShowCursor = false; }

    const int GetIndex() const
    {
        return m_Params.currentX + (m_Params.currentY * m_Params.columns);
    } //  Grabs the current index

    // Must process inputs for keyboard events
    void ProcessInputs();
    void Draw();

private:
    Console &m_Console;
    Keyboard &m_Keyboard;


    // std::function is a template class that can encapsulate any callable object matching a specific function signature. 
    // This flexibility makes it an essential tool for scenarios where you need to pass functions as parameters, store them in containers, or manage callbacks.
    // Function Signature: Specifies the return type and the types of its params
    // Example: int add(int, int), its function signature is int(int, int)

    //void is the return type, and int and std::vector<T> are the return params. Int will be the index of the data
    std::function<void(int, std::vector<T>)> m_OnSelection;

    // Selecting the data which we will draw - void(x, y, data)
    std::function<void(int, int, T)> m_OnDrawItem;
    std::vector<T> m_Data;
    SelectorParams m_Params;
    bool m_bShowCursor; //Hide or Show cursor when needed
    int m_Rows;         // Determined by the number of cols and size of data

    void MoveUp(); // Moves cursor up, down, left, right
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void OnAction();

    void DrawItem(int x, int y, T item);
    // Our default if we do not provide a new function
    void OnSelection(int index, std::vector<T> data); // Default selection
};


template <typename T>
inline Selector<T>::Selector(Console &console, Keyboard &keyboard, std::vector<T> data, SelectorParams params)
    : Selector(console, keyboard, [this](int index, std::vector<T> data)
               {
                   Selector::OnSelection(index, data);
               }, [this](int x, int y, T item)
               {
                   Selector::DrawItem(x, y, item);
               }, data, params)
{}

template <typename T>
inline Selector<T>::Selector(Console &console,
                             Keyboard &keyboard,
                             std::function<void(int, std::vector<T>)> on_selection,
                             std::function<void(int, int, T)> on_draw_item,
                             std::vector<T> data,
                             SelectorParams params)
    : m_Console(console)
    , m_Keyboard(keyboard)
    , m_OnSelection(on_selection)
    , m_OnDrawItem(on_draw_item)
    , m_Data(data)
    , m_Params(params)
    , m_bShowCursor(true)
{
    // Initialize the number of rows - Uses a Ternary Operator (short if statement, essentially)
    // Ternary Operator: condition ? expression1 : expression2; If true, execute expression 1, if false execute expression 2 
    // If params.columns == 0 then return 1, if false then return params.columns
    m_Rows = std::ceil(m_Data.size() / (params.columns == 0 ? 1 : params.columns));

    // Check to see if rows is < 1
    if (m_Rows < 1)
        m_Rows = 1;
}

template <typename T>
inline Selector<T>::~Selector()
{}

template <typename T>
inline void Selector<T>::ProcessInputs()
{
    if (m_Keyboard.IsKeyPressed(KEY_UP))
        MoveUp();
    else if (m_Keyboard.IsKeyPressed(KEY_DOWN))
        MoveDown();
    else if (m_Keyboard.IsKeyPressed(KEY_LEFT))
        MoveLeft();
    else if (m_Keyboard.IsKeyPressed(KEY_RIGHT))
        MoveRight();
    else
        if (m_Keyboard.IsKeyPressed(KEY_ENTER))
            OnAction();
}

template <typename T>
inline void Selector<T>::MoveUp()
{
    m_Params.currentY = std::max(m_Params.currentY - 1, 0);
    // If max goes current -1, i.e., if max is 0 -1, then -1 defaults to 0.
}

template <typename T>
inline void Selector<T>::MoveDown()
{
    m_Params.currentY = std::min(m_Params.currentY + 1, m_Rows - 1);
}

template <typename T>
inline void Selector<T>::MoveLeft()
{
    m_Params.currentX = std::max(m_Params.currentX - 1, 0);
}

template <typename T>
inline void Selector<T>::MoveRight()
{
    m_Params.currentX = std::min(m_Params.currentX + 1, m_Params.columns - 1);
}

// Get current index that cursor is pointing at, then call the OnSelection() function for that index
template <typename T>
inline void Selector<T>::OnAction()
{
    int index = GetIndex();
    m_OnSelection(index, m_Data);
}

// Check to see if type is w_string and draw. If not a wstring, then function must be overridden
template <typename T>
inline void Selector<T>::DrawItem(int x, int y, T item)
{
    // Check to see if the type is wstring
    if constexpr (std::is_same<T, std::wstring>::value)
    {
        m_Console.Write(x, y, item);
        // We know it's a wstring, so we allow it to draw
        return;
    }

    // If not wstring, get the type and let user know to override
    std::string type = typeid(item).name();
    CRPG_ERROR("Data type [" + type + "] - needs to be a wstring. Please create the DrawItem function to override!")
}


// OnSelection() should be overriden
template <typename T>
inline void Selector<T>::OnSelection(int index, std::vector<T> data)
{
    m_Console.Write(50, 20, L"Index: " + std::to_wstring(index));
}


template <typename T>
inline void Selector<T>::Draw()
{
    // If no data to draw, then return
    if (m_Data.empty())
        return;

    // Index when drawing begins
    int itemIndex = 0;
    int x         = m_Params.x;
    int y         = m_Params.y;
    int rowHeight = m_Params.spacingY; // vertical spacing between rows
    int spacingX  = m_Params.spacingX; // horizontal spacing between items

    int maxData = m_Data.size();

    // We want to loop over all rows and columns, and draw based on the index we're currently at
    for (int i = 0; i < m_Rows; i++)
    {
        // for loop dealing with columns
        for (int j = 0; j < m_Params.columns; j++)
        {
            if (i == m_Params.currentY && j == m_Params.currentX)
            {
                // This is where the cursor is, if these two conditions are TRUE
                if (m_bShowCursor)
                {
                    // Clear the cell above the current cursor position, if not on the first row.
                    if (m_Params.currentY != 0)
                        m_Console.Write(x - (x == 0 ? 0 : 2), y - rowHeight, L" ");

                    // Clear the cell to the left of the current cursor position, if not in the first column.
                    if (m_Params.currentX != 0)
                        m_Console.Write(x - (x == 0 ? 0 : 2) - spacingX, y, L" ");

                    // Clear the cell below the current cursor position, if not on the last row.
                    if (m_Params.currentY != m_Rows - 1)
                        m_Console.Write(x - (x == 0 ? 0 : 2), y + rowHeight, L" ");

                    // Clear the cell to the right of the current cursor position, if not in the last column.
                    if (m_Params.currentX != m_Params.columns - 1)
                        m_Console.Write(x - (x == 0 ? 0 : 2) + spacingX, y, L" ");

                    // **Draw the Cursor**
                    // Render the cursor symbol at the current (x, y) position with the specified color.
                    m_Console.Write(x - (x == 0 ? 0 : 2), y, m_Params.cursor, RED);
                }
                else
                {
                    // If m_bShowCusor == FALSE, then clear the cursor. Ternary operator to ensure we don't subtract from 0
                    //m_Console.Write(x - (x == 0 ? 0 : 2), y, L" ");
                    // **Hide the Cursor**

                    // If the cursor should not be displayed, clear the current cell.
                    m_Console.Write(x - (x == 0 ? 0 : 2), y, L" ");
                }
            }
            if (itemIndex < maxData)
            {
                // MaxData is based on the data size, so if our index is beyond the maxData size, we don't want to enter this if statement, because we would be trying to
                // grab the index of a vector that is beyond the size of the vector
                T item = m_Data[itemIndex];
                m_OnDrawItem(x, y, item);
                x += spacingX; // moved to next horizontal position
                itemIndex++;   // Increments the index
            }
        }
        // move to next row | dealing with rows now - essentially, the second for loop was iterating through the columns on row 0, we want to then move down to row 1 and do the same.
        y += rowHeight; // Increment y by spacingY for the next row
        x = m_Params.x; // Reset x to the starting position
        //std::cout << "y increment y: [" << y << "] by rowHeight: [" << rowHeight << "]" << std::endl;
        //std::cout << "x pos: [" << x << "]" << std::endl;
    }
}
