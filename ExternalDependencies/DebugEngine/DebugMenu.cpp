#include "DebugMenu.h"
DebugMenu DebugMenu::DebugMenuInstance;

#ifdef Debug
#include <QtGui\qhboxlayout>
#include <QtGui\qlabel.h>
#include <QtGui\qcheckbox.h>
#include "Logger.h"
#include <Qt\qdebug.h>


void DebugMenu::initialize(QVBoxLayout* debugLayout)
{
	nextBoolWatcher = 0;
	nextFloatSlider = 0;
	nextFloatWatcher = 0;

	mainLayout = debugLayout;
	mainLayout->addLayout(floatLayout = new QVBoxLayout);
	mainLayout->addLayout(sliderLayout = new QHBoxLayout);
	mainLayout->addLayout(checkLayout = new QHBoxLayout);
}

void DebugMenu::watchFloat(const char* description, const float* theFloat)
{
	floatLayout->addWidget(new QLabel(description));
	FloatWatchInfo& w = floatWatchers[nextFloatWatcher++]; 
	floatLayout->addWidget(w.theLabel = new QLabel);
	w.theFloat = theFloat;
}

void DebugMenu::slideFloat(const char* description, float* theFloat)
{
	sliderLayout->addWidget(new QLabel(description));
	FloatSlideInfo& w = floatSliders[nextFloatSlider++]; 
	w.slidable = true;
	sliderLayout->addWidget(w.theSlider = new Neumont::Slider(0, 1));
	w.theSlider->setValue(0);
	w.theFloat = theFloat;
}

void DebugMenu::watchCheckBox(const char* description, bool* theBool)
{
	checkLayout->addWidget(new QLabel(description));
	BoolWatchInfo& w = boolWatchers[nextBoolWatcher++];
	checkLayout->addWidget(w.theCheck = new QCheckBox );
	w.theBool = theBool;
}

void DebugMenu::update()
{
	for(unsigned int i = 0; i < nextFloatWatcher; i++)
	{
		const FloatWatchInfo& f = floatWatchers[i];
		QString temp;
		temp.setNum(*(f.theFloat));
		f.theLabel->setText(temp);
	}

	for(unsigned int i = 0; i < nextFloatSlider; i++)
	{
		const FloatSlideInfo& s = floatSliders[i];
		if(s.slidable)
		{
			*(s.theFloat) = s.theSlider->value();
		}
	}

	for(unsigned int i = 0; i < nextBoolWatcher; i++)
	{
		const BoolWatchInfo& f = boolWatchers[i];
		*f.theBool = f.theCheck->isChecked();
	}
}



#endif

DebugMenu::DebugMenu()
{

}

DebugMenu& DebugMenu::getTheInstance()
{
	return DebugMenuInstance;
}

