#include "Logger.h"
#include <fstream>

static std::ofstream outfile;
Logger Logger::loggerInstance;

Logger::Logger()
{
}

Logger Logger::getInstance()
{
	return loggerInstance;
}

void Logger::initialize()
{
	outfile.open("LoggerFile.html");
}

void Logger::shutdown()
{
	outfile.close();
}

void Logger::logInitialize(const char* message, const char* fileName, int lineNumber)
{
	outfile <<"<br> <il style = \"color:black\">"
	<< message << "<br> File Name: " 
	<< fileName << "<br> Line Number: " 
	<< lineNumber << "</li> <br>"; 

	outfile.flush();
}

void Logger::InternaLog(const char* message, const char* fileName, int lineNumber)
{
	outfile <<"<br> <il style = \"color:red\">"
	<< message << "<br> File Name: " 
	<< fileName << "<br> Line Number: " 
	<< lineNumber << "</li> <br>"; 

	outfile.flush();
}
