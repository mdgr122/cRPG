#pragma once

#include <iostream>
#include <string_view>
#include <source_location>

#define CRPG_LOG(x) Logger::Log(x);
#define CRPG_ERROR(x) Logger::Error(x);

class Logger
{
public:
	Logger() {};
	~Logger() = default; // Default destructor

	static void Log(const std::string_view message);
	static void Error(const std::string_view message, std::source_location location = std::source_location::current());

private:
	static std::string CurrentDate();

};