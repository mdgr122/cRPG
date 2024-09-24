#pragma once

#include <iostream>
#include <string_view>
#include <source_location>

class Logger
{
public:
	Logger() {};
	~Logger() = default; // Default destructor

	static void Log(const std::string_view message);
	static void Error(const std::string_view, std::source_location = std::source_location::current());

private:
	static std::string CurrentDate();

};