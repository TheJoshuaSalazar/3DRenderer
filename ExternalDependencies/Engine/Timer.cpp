//#include "Timer.h"
//#define WIN32_LEAN_AND_MEAN
//
//
//LARGE_INTEGER Timer::frequency;
//
//
//Timer::Timer()
//{
//	QueryPerformanceFrequency(&frequency);
//}
//void Timer::Start()
//{
//	QueryPerformanceCounter(&start);
//	curret = start;
//	last = start;
//}
//
//float Timer::Stop()
//{
//	QueryPerformanceCounter(&end);
//	return (float)(end.QuadPart-start.QuadPart)/frequency.QuadPart;
//}
//
//float Timer::Interval()
//{
//	last = curret;
//	QueryPerformanceCounter(&curret);
//	return (float)(curret.QuadPart-last.QuadPart)/frequency.QuadPart;
//}