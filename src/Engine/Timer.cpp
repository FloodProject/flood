/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
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
	: currentTime(0)
	, lastTime(0)
{
	if( !checked && !checkHighResolutionTimers() )
		Log::error( "High-resolution timers are not supported" );

	reset();
}

//-----------------------------------//

double Timer::getCurrentTime()
{
	storeTime(currentTime);

#ifdef VAPOR_PLATFORM_WINDOWS	
	return (double) currentTime / (double) ticksPerSecond;
#else
	return currentTime.tv_sec;
#endif
}

//-----------------------------------//

double Timer::getElapsedTime()
{
	getCurrentTime();	

#ifdef VAPOR_PLATFORM_WINDOWS
	Ticks diff = currentTime - lastTime;
	return (double) diff / (double) ticksPerSecond;
#else
	return currentTime.tv_sec - lastTime.tv_sec;
#endif
}

//-----------------------------------//

double Timer::reset()
{
	storeTime(lastTime);

#ifdef VAPOR_PLATFORM_WINDOWS
	return (double) lastTime / (double) ticksPerSecond;
#else
	return lastTime.tv_sec;
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