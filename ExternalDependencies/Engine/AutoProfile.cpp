#include <stdio.h>
#include "AutoProfile.h"
#include "Timer.h"

int Profiler::curretFrame = 0;
float Profiler::Samples[MAX_FRAMES][MAX_CATEGORIES];

AutoProfile::AutoProfile()
{}

AutoProfile::AutoProfile(int number)
{
	name = number;
	times.Start();
}

AutoProfile::~AutoProfile()
{
	float LapseTime = times.Stop();
	Profiler::AddSample(name, LapseTime);
}

void Profiler::AddSample(int num,float value)
{
	if(curretFrame < MAX_FRAMES)
	{
		Profiler::Samples[curretFrame][num] = value;
		if(num >= MAX_CATEGORIES-1)
		{
			curretFrame++;
		}
	}
}

void Profiler::CSV(const char* file)
{
	FILE *GameFile = NULL;
	fopen_s(&GameFile, file, "w");
	if(GameFile != NULL)
	{
		for(int i =0;i<MAX_CATEGORIES; i++)
		{
			if(i==0)
				fprintf(GameFile,"EnemyShip,",i);
			if(i==1)
				fprintf(GameFile,"Particles,",i);
			if(i==2)
				fprintf(GameFile,"SpaceShip,",i);
			if(i==3)
				fprintf(GameFile,"Bullet,",i);
			if(i==4)
				fprintf(GameFile,"EnemyDraw,",i);
			if(i==5)
				fprintf(GameFile,"ParticlesDraw,",i);
			if(i==6)
				fprintf(GameFile,"SpaceShipDraw,",i);
			if(i==7)
				fprintf(GameFile,"BulletDraw",i);
			
		}
		fprintf(GameFile,"\n ");

		for(int i = 0; i<MAX_FRAMES; i++)
		{
			for(int j = 0; j<MAX_CATEGORIES; j++)
			{
				fprintf(GameFile,"%f, ", Profiler::Samples[i][j]);
			}
			fprintf(GameFile,"\n ");
		}
		fclose(GameFile);
	}

	else
	{
		printf("file %s fail to open.\n", file);
	}
}