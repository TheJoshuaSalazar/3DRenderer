#ifndef CLOCK
#define CLOCK
#include "Windows.h"

#define WIN32_LEAN_AND_MEAN
#include "ExportHeader.h"


class ENGINE_SHARED Clock
{
public:
	Clock();
	bool initialize();
	void start();
	void stop();
	bool shutdown();
	void nextFrame();
	float timeElapsedLastFrame() const;
	static Clock& getInstance();

private:
	Clock(const Clock&);
	Clock operator=(const Clock&);
	static Clock clockInstance;

	LARGE_INTEGER timeFrequency;
	LARGE_INTEGER deltaLastLap;
	LARGE_INTEGER lastStartTime;
	float deltaTime;
};

#define myClock Clock::getInstance()

#endif