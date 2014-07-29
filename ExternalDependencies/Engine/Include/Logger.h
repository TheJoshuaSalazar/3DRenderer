#ifndef LOGGER_H
#define LOGGER_H

#include "ExportHeader.h"


class Logger
{
	static Logger instance;
	Logger() {}

public:
	bool initialize(char* fileName = "log.html");
	bool shutdown();
	void InternaLog(const char* message, const char* flieName, int lineNumber);
	static Logger& getInstance() 
	{
		return instance;
	}
	static void Log(const char* message, const char* fileName, int lineNumber)
	{
		instance.InternaLog(message, fileName, lineNumber);
	}

};

#define LOG(message) \
		Logger::Log(message,__FILE__,__LINE__)

#endif //Logger
 