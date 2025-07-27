#include "utilities/Typewriter.h"
#include <algorithm>
#include "Console.h"
#include "Logger.h"
#include "utilities/Globals.h"
#include "utilities/crpg_utils.h"


TypeWriter::TypeWriter(Console &console)
    : TypeWriter(console, 1, 1, L"Set the type writer text", 50, 250)
{}

TypeWriter::TypeWriter(Console &console, int start_x, int start_y, const std::wstring &text, int text_wrap, int text_speed, WORD textColor, WORD borderColor)
    : m_Console(console)
    , m_sText{text}
    , m_sCurrentText{L""}
    , m_X{start_x}
    , m_Y{start_y}
    , m_BorderX{0}
    , m_BorderY{0}
    , m_BorderWidth{0}
    , m_BorderHeight{0}
    , m_TextSpeed{text_speed}
    , m_TextWrap{text_wrap}
    , m_Index{0}
    , m_CharIndex{0}
    , m_TextIndex{0}
    , m_TextColor{textColor}
    , m_BorderColor{borderColor}
    , m_Timer{ }
    , m_bFinished{false}

{
    if (!SetText(text))
    {
        CRPG_ERROR("Failed to  initialize text for the typewriter");
        return;
    }
    ClearArea();
    m_Timer.Start();
}

TypeWriter::~TypeWriter()
{}


// Processes a given wstring by splitting it into smaller chunks based on the set text wrap length (m_TextWrap).
// Each chunk is added to m_sTextChunks, which will be used by the hConsole buffer for typewriter-style display.
// The function evaluates the length of the current line buffer and determines whether to add a word to the current line or start a new line.
// Long words that do not fit within the remaining space are split appropriately, ensuring that each chunk respects the text wrap length limit.
// Conditions are optimized to ensure proper handling of exact line fits, long word splits, and efficient buffer management.
// Finally, SetBorderProperties() is called to configure text display properties.
bool TypeWriter::SetText(const std::wstring &text)
{
    m_sText = text;
    m_sTextChunks.clear();
    std::wstringstream wss(text);
    std::wstring text_word;
    std::vector<std::wstring> text_words;
    std::wstring line_buffer;
    std::vector<std::wstring> processed_lines;


    while (wss >> text_word)
    {
        text_words.push_back(text_word);
    }

    std::wstring split_first  = L"";
    std::wstring split_second = L"";
    for (auto &elem : text_words)
    {
        size_t split_pos{ };
        size_t line_buff_length = line_buffer.length();
        if (line_buffer.length() + elem.length() + 1 <= m_TextWrap)
        {
            if (!line_buffer.empty())
            {
                line_buffer += L" ";
            }
            line_buffer += elem;
        }
        else
        {
            // If current line_buffer length and word length equal exactly 60, then create a new line.
            if (line_buffer.length() + elem.length() + 1 == m_TextWrap)
            {
                size_t line_elem_size = line_buffer.length() + elem.length();
                if (!line_buffer.empty())
                {
                    line_buffer += L" ";
                }
                line_buffer += elem;
                break;
            }
            else if (elem.length() >= 7 && (line_buffer.length() != m_TextWrap) && (line_buffer.length() + elem.length() + 1) > m_TextWrap && (m_TextWrap - line_buffer.length() >
                4))
            {
                // If word length is greater than 8, and the buffer + word length is greater than the textwrap length.
                split_first = L"";

                split_pos = m_TextWrap - (line_buffer.length() + 2);

                auto split_strings = CRPG_Globals::GetInstance().wstr_split_to_wstr(elem, split_pos);
                split_first        = split_strings.first + L"-";
                split_second       = split_strings.second;

                line_buffer += (L" " + split_first);

                //processed_lines.push_back(line_buffer);
                m_sTextChunks.push_back(line_buffer);
                line_buffer = split_second;
            }
            else
            {
                if (split_second != L"")
                {
                    line_buffer  = split_second;
                    split_second = L"";
                }
                else
                {
                    //processed_lines.push_back(line_buffer);
                    m_sTextChunks.push_back(line_buffer);
                    line_buffer = elem;
                }
            }
            split_second.clear();
        }
    }

    // Ensure any remaining split_second or line_buffer content is pushed
    if (!line_buffer.empty())
    {
        //processed_lines.push_back(line_buffer);
        m_sTextChunks.push_back(line_buffer);
    }

    //std::wcout << "\nProcessed Lines:\n";
    //for (const auto& elem : m_sTextChunks)
    //{
    //	std::wcout << elem << " ";
    //}
    //std::cout << std::endl;

    if (!SetBorderProperties())
    {
        CRPG_ERROR("Failed to set the border properties!");
        return false;
    }


    return true;
}

void TypeWriter::UpdateText()
{
    if (!m_Timer.IsRunning() || m_bFinished)
        return;

    if (m_Timer.ElapsedMS() > m_TextSpeed * m_Index && m_TextIndex < m_sTextChunks.size() && m_Index < m_sText.size())
    {
        m_sCurrentText += m_sTextChunks[m_TextIndex][m_CharIndex];
        //std::wcout << "sCurrentText: " << m_sCurrentText << std::endl;
        if (m_CharIndex >= m_sTextChunks[m_TextIndex].size())
        {
            //std::wcout << "sCurrentText: " << m_sCurrentText << std::endl;
            m_CharIndex = 0;
            m_TextIndex++;
            m_Y++;
            m_sCurrentText.clear();
        }
        else
        {
            m_CharIndex++;
            m_Index++;
        }
    }

    if (m_Index >= m_sText.size())
    {
        m_Timer.Stop();
        m_bFinished = true;
    }
}

void TypeWriter::Draw(bool showborder)
{
    if (m_Timer.IsRunning())
    {
        m_Console.Write(m_X, m_Y, m_sCurrentText, m_TextColor);

        if (showborder)
            DrawBorder();
    }
}

bool TypeWriter::SetBorderProperties()
{
    m_BorderX = std::clamp(m_X - 2, 0, 127);
    // std::clamp(value, low, high);. If value < low, then low returned. If Value > high, then high returned. If low < value < high, then value is returned
    m_BorderY = std::clamp(m_Y - 2, 0, 47);

    m_BorderWidth  = m_TextWrap + 2;
    m_BorderHeight = m_sTextChunks.size() + 2;

    if (m_BorderHeight <= 2 || m_BorderWidth <= 2)
    {
        CRPG_ERROR("Border Height/Width is too small!");
        return false;
    }

    if (m_BorderX + m_TextWrap + 2 > 127 || m_BorderY + m_sTextChunks.size() + 2 > 47)
    {
        CRPG_ERROR("Border x or border y write beyond the buffer size!");
        return false;
    }

    return true;
}

void TypeWriter::DrawBorder()
{
    m_Console.DrawPanel(m_BorderX, m_BorderY, m_BorderWidth + 1, m_BorderHeight + 1, m_BorderColor);
}

void TypeWriter::ClearArea()
{
    for (int i = 0; i <= m_BorderHeight; i++)
    {
        for (int j = 0; j <= m_BorderWidth; j++)
        {
            m_Console.Write(m_BorderX + j, m_BorderY + i, L" ");
        }
    }
}
