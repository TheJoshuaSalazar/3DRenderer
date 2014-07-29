#pragma once
#ifndef DEBUG_MENU
#define DEBUG_MENU
#include "Slider.h"
#include "ExportHeader.h"

#pragma warning(disable: 4100)

class QVBoxLayout;

#define Debug

#ifdef Debug
class QCheckBox;
class QHBoxLayout;
class QLabel;

namespace Neumont {class Slider;}

class __declspec( dllexport ) DebugMenu
{	
	QVBoxLayout* mainLayout;
	QVBoxLayout* floatLayout;
	QHBoxLayout* sliderLayout;
	QHBoxLayout* checkLayout;

	struct FloatWatchInfo
	{
		const float* theFloat;
		QLabel* theLabel;
	} floatWatchers[20];
	unsigned int nextFloatWatcher;

	struct FloatSlideInfo
	{
		float* theFloat;
		QLabel* theLabel;
		Neumont::Slider* theSlider;
		bool slidable;
		FloatSlideInfo() : slidable(false) {}
	} floatSliders[20];
	unsigned int nextFloatSlider;

	struct BoolWatchInfo
	{
		bool* theBool;
		QLabel* theLabel;
		QCheckBox* theCheck;
	} boolWatchers[20];
	unsigned int nextBoolWatcher;

	DebugMenu();
	DebugMenu(const DebugMenu&);
	DebugMenu operator=(const DebugMenu&);

public:

	void initialize(QVBoxLayout* debugLayout);
	void watchFloat(const char* description, const float* theFloat);
	void slideFloat(const char* description, float* theFloat);
	void watchCheckBox(const char* description, bool* theBool);
	void update();
	void removeLayouts();

	//DebugMenu() : nextFloatWatcher(0) {}
	static DebugMenu& getTheInstance();

private:
	
	static DebugMenu DebugMenuInstance;
};


#else
	class __declspec( dllexport ) DebugMenu
{	
	DebugMenu();
	DebugMenu(const DebugMenu&);
	DebugMenu operator=(const DebugMenu&);
	static DebugMenu DebugMenuInstance;

public:

	void initialize(QVBoxLayout* debugLayout){}
	void watchFloat(const char* description, const float* theFloat){}
	void slideFloat(const char* description, float* theFloat){}
	void watchCheckBox(const char* description, bool* theBool){}
	void update(){}
	void removeLayouts(){}
	static DebugMenu& getTheInstance();
};

#endif

	#define myDebugMenu DebugMenu::getTheInstance()

#endif

