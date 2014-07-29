#ifndef TIMER_H
#define TIMER_H


class Timer
{
public:
	Timer();
	void Start();
	float Stop();
	float Interval();

private:	 
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER curret;
	LARGE_INTEGER last;
	static LARGE_INTEGER frequency;
};


#endif //Timmer.h