#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Constants.h"

Logger::Logger()
{
	//Create desired directory if not present
	if (!std::filesystem::exists(DIR_LOGS))
	{
		try
		{
			std::filesystem::create_directories(DIR_LOGS);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: Could not create directory \"" << DIR_LOGS << "\". Exception: " << e.what() << std::endl;
		}
	}

	//Open the log output file
	mFile.open(FILE_LOG.c_str(), std::ios::out);
	if (!mFile.is_open())
		std::cerr << "Error: Could not open file \"" << FILE_LOG << '"' << std::endl;
}

constexpr const char* Logger::levelToString(LogLevel level)
{
	//Return desired string representation based on log level
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
	//Thread safety
	std::lock_guard<std::mutex> lock(mMutex);

	//Get current timestep
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

	//Get source filename
	std::string filename = std::filesystem::path(file).filename().string();

	//Build full message and log it
	std::string fullMessage = date.str() + " [" + levelToString(level) + "] " +
		message + " (" + filename + ':' + std::to_string(line) + ')';

	if (mFile.is_open())
		mFile << fullMessage << std::endl;

	//Print to console if Debug build
#ifdef _DEBUG
	std::cout << fullMessage << std::endl;
#endif
}