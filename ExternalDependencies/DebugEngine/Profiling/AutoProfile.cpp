#include <stdio.h>
#include "AutoProfile.h"

int Profilers::curretFrame = 0;
float Profilers::Samples[MAX_FRAMES][MAX_CATEGORIES];

AutoProfile AutoProfile::AutoProfilerInstance;

AutoProfile& AutoProfile::getProfilerInstance()
{ 
	return AutoProfilerInstance;
}


AutoProfile::AutoProfile()
{}

AutoProfile::AutoProfile(int number)
{
	name = number;
	clock.initialize();
	clock.start();
}

AutoProfile::~AutoProfile()
{
	clock.stop();
	Profilers::AddSample(name, clock.timeElapsedLastFrame());
}

void Profilers::AddSample(int num,float value)
{
	if(curretFrame < MAX_FRAMES)
	{
		Profilers::Samples[curretFrame][num] = value;
		if(num >= MAX_CATEGORIES - 1)
		{
			curretFrame++;
		}
	}
}

void Profilers::CSV(const char* file)
{
	FILE *GameFile = NULL;
	fopen_s(&GameFile, file, "w");
	if(GameFile != NULL)
	{
		for(int i = 0; i <MAX_CATEGORIES; i++)
		{
			if(i==0)
				fprintf(GameFile,"Collision,",i);
			if(i==1)
				fprintf(GameFile,"Draw,",i);
			
		}
		fprintf(GameFile,"\n ");

		for(int i = 0; i<MAX_FRAMES; i++)
		{
			for(int j = 0; j<MAX_CATEGORIES; j++)
			{
				fprintf(GameFile,"%f, ", Profilers::Samples[i][j]);
			}
			fprintf(GameFile,"\n ");
		}
		fclose(GameFile);
	}

}