#ifndef LOGGER_H
#define LOGGER_H

#include "ExportHeader.h"

class __declspec( dllexport ) Logger 
{
	Logger();
	Logger operator=(const Logger&);
	static Logger loggerInstance;

public:
	void initialize();
	void shutdown();
	void logInitialize(const char* message, const char* flieName, int lineNumber);
	void InternaLog(const char* message, const char* flieName, int lineNumber);
	
	static Logger getInstance();

};

#define LOG(message) \
		Logger::Log(message,__FILE__,__LINE__)

#endif //Logger
 

