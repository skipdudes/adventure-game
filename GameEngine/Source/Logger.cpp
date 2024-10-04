#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem>

Logger::Logger()
{
	mFile.open(mFilename.c_str(), std::ios::out);
	if (!mFile.is_open())
	{
		std::cerr << "Error: Could not open the log file (" << mFilename << ')' << std::endl;
	}
}

Logger::~Logger()
{
	if (mFile.is_open())
		mFile.close();
}

std::string Logger::levelToString(LogLevel level)
{
	switch (level)
	{
	case INFO:
		return "INFO";
	case WARNING:
		return "WARNING";
	case ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}

void Logger::log(LogLevel level, const std::string& message, const std::string& file, int line)
{
	std::lock_guard<std::mutex> lock(mMutex);

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
	
#ifdef _MSC_VER
	localtime_s(&now_tm, &now_time_t);
#else
	now_tm = *std::localtime(&now_time_t);
#endif

	std::stringstream date;
	date << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
	
	std::string filename = std::filesystem::path(file).filename().string();
	std::string fullMessage = date.str() + " [" + levelToString(level) + "] " + message + " (" + filename + ':' + std::to_string(line) + ')';
	mFile << fullMessage << std::endl;
	mFile.flush();

#ifdef _DEBUG
	std::cout << fullMessage << std::endl;
#endif
}
