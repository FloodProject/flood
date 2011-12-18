/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Timer.h"
#include "Core/Log.h"
#include "Core/Memory.h"

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#undef  NOMINMAX
	#define NOMINMAX
	#include <Windows.h>	
#elif PLATFORM_MACOSX
	#pragma TODO("OSX: Use Mach timers: http://developer.apple.com/library/mac/#qa/qa2004/qa1398.html")
#else
	#include <sys/time.h>
	#define tv_time_ms(t) ((t.tv_sec * 1000000.0) + t.tv_usec)
	#pragma TODO("Linux: http://linux.die.net/man/3/clock_gettime")
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

#ifdef PLATFORM_WINDOWS

static bool CheckHighResolution();

static int64 g_TicksPerSecond;
static int g_HighResolutionSupport = CheckHighResolution();

static bool CheckHighResolution()
{
	LARGE_INTEGER* frequency = (LARGE_INTEGER*) &g_TicksPerSecond;
	return QueryPerformanceFrequency(frequency) != 0;
}

static int64 GetTime()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

#elif PLATFORM_LINUX

static timeval GetTime()
{
	timeval time;
	gettimeofday(&time, nullptr);
	return time;
}

#endif

//-----------------------------------//

float TimerGetCurrentTimeMs()
{
#ifdef PLATFORM_WINDOWS
	int64 time = GetTime();
	return float(time) / float(g_TicksPerSecond);
#else
	timeval time = GetTime();
	return tv_time_ms(time);
#endif
}

//-----------------------------------//

Timer::Timer()
{
	TimerReset(this);
}

//-----------------------------------//

Timer* TimerCreate(Allocator* alloc)
{
	Timer* timer = Allocate(alloc, Timer);
	TimerReset(timer);

	return timer;
}

//-----------------------------------//

void TimerDestroy(Timer* timer)
{
	Deallocate(timer);
}

//-----------------------------------//

void TimerReset(Timer* timer)
{
	timer->time = GetTime();
}

//-----------------------------------//

float TimerGetElapsed(Timer* timer)
{
	int64 diff = GetTime() - timer->time;

#ifdef PLATFORM_WINDOWS
	return float(diff) / float(g_TicksPerSecond);
#else
	return tv_time_ms(currentTime) - tv_time_ms(lastTime.tv_sec);
#endif
}

//-----------------------------------//

NAMESPACE_CORE_END