#include "Clock.h"

Clock Clock::clockInstance;

Clock::Clock()
{

}

bool Clock::initialize()
{
	bool b = QueryPerformanceFrequency(&timeFrequency) !=0 ;
	if(! b ) 
		return false;

	return QueryPerformanceCounter(&lastStartTime) !=0 ;
}

bool Clock::shutdown()
{ 
	return true; 
}

void Clock::start()
{
	QueryPerformanceCounter(&lastStartTime);
}

void Clock::stop()
{
	LARGE_INTEGER thisTime;
	QueryPerformanceCounter(&thisTime);
	LARGE_INTEGER delta;
	delta.QuadPart = thisTime.QuadPart - lastStartTime.QuadPart;
	deltaTime = ((float)delta.QuadPart) / timeFrequency.QuadPart;
	deltaLastLap.QuadPart = thisTime.QuadPart;
}

void Clock::nextFrame()
{
	stop();
	start();
}

float Clock::timeElapsedLastFrame() const
{
	return deltaTime;
}

Clock& Clock::getInstance()
{
	return clockInstance;
}