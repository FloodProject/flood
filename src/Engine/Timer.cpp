/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Timer.h"

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	ticks_t Timer::ticksPerSecond = 0;
#endif

bool Timer::checked = false;
bool Timer::highResolutionSupport = false;

//-----------------------------------//

Timer::Timer()
#ifdef VAPOR_PLATFORM_WINDOWS
	: currentTime(0)
	, lastTime(0)
#else
	// Figure out what to put in here
#endif
{
	if( !checked && !checkHighResolutionTimers() )
		error( "timer", "High-resolution timers are not supported" );

	reset();
}

//-----------------------------------//

double Timer::getCurrentTime()
{
	storeTime(currentTime);
#ifdef VAPOR_PLATFORM_WINDOWS	
	return static_cast<double>(currentTime)
		/ static_cast<double>(ticksPerSecond);
#else
	return currentTime.tv_sec;
#endif
}

//-----------------------------------//

double Timer::getElapsedTime()
{
	getCurrentTime();	

#ifdef VAPOR_PLATFORM_WINDOWS
	ticks_t diff = currentTime - lastTime;
	return static_cast<double>(diff) 
		/ static_cast<double>(ticksPerSecond);
#else
	return currentTime.tv_sec - lastTime.tv_sec;
#endif
}

//-----------------------------------//

double Timer::reset()
{
	storeTime(lastTime);
#ifdef VAPOR_PLATFORM_WINDOWS
	return static_cast<double>(lastTime)
		/ static_cast<double>(ticksPerSecond);
#else
	return lastTime.tv_sec;
#endif
}

//-----------------------------------//

void Timer::storeTime( ticks_t& var )
{
#ifdef VAPOR_PLATFORM_WINDOWS

	LARGE_INTEGER* time = nullptr;
	time = reinterpret_cast<LARGE_INTEGER*>( &var );
	
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

	LARGE_INTEGER* freq = nullptr;
	freq = reinterpret_cast<LARGE_INTEGER*>( &ticksPerSecond );
	
	if( !QueryPerformanceFrequency(freq) )
		return false;
#endif

	return true;
}

//-----------------------------------//

void Timer::sleep( double time )
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