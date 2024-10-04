#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger
{
public:
	enum LogLevel
	{
		INFO,
		WARNING,
		ERROR
	};

	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;
	static Logger& getInstance();
	void log(LogLevel level, const std::string& message, const std::string& file, int line);

private:
	std::string mFilename = "Logs/latest.log";
	std::ofstream mFile;
	std::mutex mMutex;

	Logger();
	~Logger();
	std::string levelToString(LogLevel level);
};

#ifdef _DEBUG
#define LOG_INFO(message) Logger::getInstance().log(Logger::LogLevel::INFO, message, __FILE__, __LINE__)
#else
#define LOG_INFO(message)
#endif

#define LOG_WARNING(message) Logger::getInstance().log(Logger::LogLevel::WARNING, message, __FILE__, __LINE__)
#define LOG_ERROR(message) Logger::getInstance().log(Logger::LogLevel::ERROR, message, __FILE__, __LINE__)
