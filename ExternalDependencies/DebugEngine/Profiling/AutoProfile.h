#ifndef AUTOPROFILE
#define AUTOPROFILE
#include "Clock.h"

class __declspec(dllexport) AutoProfile
{
public:
	AutoProfile(int number);
	~AutoProfile();
	static AutoProfile& getProfilerInstance();

private:
	AutoProfile();
	AutoProfile(const AutoProfile&);
	AutoProfile& operator=(const AutoProfile&);
	static AutoProfile AutoProfilerInstance;
	int name;
	Clock clock;
};

class __declspec(dllexport) Profilers
{
public:
	static const int MAX_FRAMES = 1000;
	static const int MAX_CATEGORIES = 2;
	static void AddSample(int num, float value);
	static void CSV(const char* file);

	static int curretFrame;
	static float Samples[MAX_FRAMES][MAX_CATEGORIES];
};

enum ProfileCategories
{
	COLLISION_UPDATE,
	DRAW_UPDATE
};

#endif