/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Timer.h"
#include "Log.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	Ticks Timer::ticksPerSecond = 0;
#endif

bool Timer::checked = false;
bool Timer::highResolutionSupport = false;

//-----------------------------------//

Timer::Timer()
#ifdef VAPOR_PLATFORM_WINDOWS
	: currentTime(0)
	, lastTime(0)
#endif
{
	if( !checked && !checkHighResolutionTimers() )
		Log::error( "High-resolution timers are not supported" );

	reset();
}

//-----------------------------------//

#define tv_time_ms(t) ((t.tv_sec * 1000000.0) + t.tv_usec)

float Timer::getCurrentTime()
{
	storeTime(currentTime);

#ifdef VAPOR_PLATFORM_WINDOWS	
	return (float) currentTime / (float) ticksPerSecond;
#else
	return tv_time_ms(currentTime);
#endif
}

#pragma TODO("OSX: Use Mach timers: http://developer.apple.com/library/mac/#qa/qa2004/qa1398.html")
// Linux: http://linux.die.net/man/3/clock_gettime

//-----------------------------------//

float Timer::getElapsedTime()
{
	getCurrentTime();	

#ifdef VAPOR_PLATFORM_WINDOWS
	Ticks diff = currentTime - lastTime;
	return (float) diff / (float) ticksPerSecond;
#else
	return tv_time_ms(currentTime) - tv_time_ms(lastTime.tv_sec);
#endif
}

//-----------------------------------//

float Timer::reset()
{
	storeTime(lastTime);

#ifdef VAPOR_PLATFORM_WINDOWS
	return (float) lastTime / (float) ticksPerSecond;
#else
	return tv_time_ms(lastTime);
#endif
}

//-----------------------------------//

void Timer::storeTime( Ticks& var )
{
#ifdef VAPOR_PLATFORM_WINDOWS
	LARGE_INTEGER* time = (LARGE_INTEGER*) &var;
	QueryPerformanceCounter( time );
#else
	gettimeofday(&var, NULL);
#endif
}

//-----------------------------------//

bool Timer::checkHighResolutionTimers()
{
	checked = true;

#ifdef VAPOR_PLATFORM_WINDOWS
	LARGE_INTEGER* freq = (LARGE_INTEGER*) &ticksPerSecond;
	
	if( !QueryPerformanceFrequency(freq) )
		return false;
#endif

	return true;
}

//-----------------------------------//

} // end namespace