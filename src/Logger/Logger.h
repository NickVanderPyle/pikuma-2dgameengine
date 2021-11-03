#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum LogType
{
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry
{
	LogType type;
	std::string message;
};
class Logger
{
public:
	static void Log(const std::string &message);
	static void Err(const std::string &message);
};


#endif //LOGGER_H
