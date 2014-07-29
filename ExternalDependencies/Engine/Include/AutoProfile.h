#ifndef AUTOPROFILE
#define AUTOPROFILE
#include "Timer.h"

class AutoProfile
{
public:
	AutoProfile();
	AutoProfile(int number);
	~AutoProfile();

private:
	int name;
	Timer times;
};

class Profiler
{
public:
	static const int MAX_FRAMES = 1000;
	static const int MAX_CATEGORIES = 8;
	static void AddSample(int num, float value);
	static void CSV(const char* file);

	static int curretFrame;
	static float Samples[MAX_FRAMES][MAX_CATEGORIES];
};

enum ProfileCategories
{
	ENEMYSHIP_UPDATE,
	PARTICLEEMITTER_UPDATE,
	SPACESHIP_UPDATE,
	BULLET_UPDATE,
	ENEMYSHIP_DRAW,
	PARTICLEEMITTER_DRAW,
	SPACESHIP_DRAW,
	BULLET_DRAW
};


#endif //Autoprofile