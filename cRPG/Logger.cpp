#include "Logger.h"
#include <chrono>
#include <ctime>
#include <Windows.h>

void Logger::Log(const std::string_view message)
{
}

void Logger::Error(const std::string_view, std::source_location)
{
}

std::string Logger::CurrentDate()
{
    return std::string();
}
