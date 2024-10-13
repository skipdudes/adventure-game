#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger
{
public:
	//Possible log levels
	enum LogLevel
	{
		INFO,
		WARNING,
		ERROR
	};

	//Singleton implementation
	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;
	static Logger& getInstance();

	//Message logging method
	void log(LogLevel level, const std::string& message, const std::string& file, int line);

private:
	//Log output file
	std::string mDirectory = "Logs";
	std::string mFilename = "latest.log";
	std::string mFilepath;
	std::ofstream mFile;
	std::mutex mMutex;

	Logger();
	constexpr const char* levelToString(LogLevel level);
};

//Info level logs only in Debug build
#ifdef _DEBUG
#define LOG_INFO(message) Logger::getInstance().log(Logger::LogLevel::INFO, message, __FILE__, __LINE__)
#else
#define LOG_INFO(message)
#endif

#define LOG_WARNING(message) Logger::getInstance().log(Logger::LogLevel::WARNING, message, __FILE__, __LINE__)
#define LOG_ERROR(message) Logger::getInstance().log(Logger::LogLevel::ERROR, message, __FILE__, __LINE__)
