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

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#elif VAPOR_PLATFORM_MACOSX
	#pragma TODO("OSX: Use Mach timers: http://developer.apple.com/library/mac/#qa/qa2004/qa1398.html")
#else
	#include <sys/time.h>
	#define tv_time_ms(t) ((t.tv_sec * 1000000.0) + t.tv_usec)
	#pragma TODO("Linux: http://linux.die.net/man/3/clock_gettime")
#endif

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS

static bool CheckHighResolution();

static int64 ticksPerSecond;
static bool highResolutionSupport = CheckHighResolution();

static bool CheckHighResolution()
{
	LARGE_INTEGER* frequency = (LARGE_INTEGER*) &ticksPerSecond;
	return QueryPerformanceFrequency(frequency) != 0;
}

static int64 GetTime()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

#elif VAPOR_PLATFORM_LINUX

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
#ifdef VAPOR_PLATFORM_WINDOWS
	int64 time = GetTime();
	return float(time) / float(ticksPerSecond);
#else
	timeval time = GetTime();
	return tv_time_ms(time);
#endif
}

//-----------------------------------//

Timer* TimerCreate(Allocator* mem)
{
	Timer* timer = Allocate<Timer>(mem);
	TimerReset(timer);

	return timer;
}

//-----------------------------------//

void TimerDestroy(Timer* timer, Allocator* mem)
{
	Deallocate(mem, timer);
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

#ifdef VAPOR_PLATFORM_WINDOWS
	return float(diff) / float(ticksPerSecond);
#else
	return tv_time_ms(currentTime) - tv_time_ms(lastTime.tv_sec);
#endif
}

//-----------------------------------//

void TimerSleep( int64 time )
{
#ifdef VAPOR_PLATFORM_WINDOWS
	::Sleep( static_cast<DWORD>(time) );
#else
	timespec param;
	param.tv_nsec = time*1000000000.0;
	nanosleep(&param, NULL);
#endif
}

//-----------------------------------//

} // end namespace