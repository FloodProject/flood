/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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
#elif defined(PLATFORM_MACOSX)
	#pragma TODO("OSX: Use Mach timers: http://developer.apple.com/library/mac/#qa/qa2004/qa1398.html")
#elif defined(PLATFORM_NACL)
	#include <ppapi/c/pp_time.h>
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

#elif defined(PLATFORM_NACL)

PP_Time GetTime();

#elif PLATFORM_LINUX

static timeval GetTime()
{
	timeval time;
	gettimeofday(&time, nullptr);
	return time;
}

#endif

//-----------------------------------//

Timer::Timer()
{
	reset();
}

//-----------------------------------//

float Timer::getCurrentTimeMs()
{
#ifdef PLATFORM_WINDOWS
	int64 time = GetTime();
	return float(time) / float(g_TicksPerSecond);
#elif defined(PLATFORM_NACL)
    PP_Time time = GetTime();
    return time;
#else
	timeval time = GetTime();
	return tv_time_ms(time);
#endif
}

//-----------------------------------//

void Timer::reset()
{
	time = GetTime();
}

//-----------------------------------//

float Timer::getElapsed()
{
#ifdef PLATFORM_WINDOWS
    int64 diff = GetTime() - time;
	return float(diff) / float(g_TicksPerSecond);
#elif defined(PLATFORM_NACL)
    int64 diff = GetTime() - time;
    return diff;
#else
	return tv_time_ms(currentTime) - tv_time_ms(lastTime.tv_sec);
#endif
}

//-----------------------------------//

NAMESPACE_CORE_END