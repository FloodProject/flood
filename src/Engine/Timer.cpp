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

ticks_t Timer::ticksPerSecond = 0;
bool Timer::checked = false;
bool Timer::highResolutionSupport = false;

//-----------------------------------//

Timer::Timer()
	: currentTime(0),
	lastTime(0)
{
	if( !checked && !checkHighResolutionTimers() )
		error( "timer", "High-resolution timers are not supported" );

	reset();
}

//-----------------------------------//

double Timer::getCurrentTime()
{
	storeTime(currentTime);
	
	return static_cast<double>( currentTime );
}

//-----------------------------------//

double Timer::getElapsedTime()
{
	getCurrentTime();	

	ticks_t diff = currentTime - lastTime;

	return static_cast<double>(diff) 
		/ static_cast<double>(ticksPerSecond);
}

//-----------------------------------//

void Timer::reset()
{
	storeTime(lastTime);
}

//-----------------------------------//

void Timer::storeTime( ticks_t& var )
{
#ifdef VAPOR_PLATFORM_WINDOWS

	LARGE_INTEGER* time = nullptr;
	time = reinterpret_cast<LARGE_INTEGER*>( &var );
	
	QueryPerformanceCounter( time );

#else
	#error "Implementation is missing"
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

#else
	#error "Implementation is missing"
#endif

	return true;
}

//-----------------------------------//

void Timer::sleep( double time )
{
#ifdef VAPOR_PLATFORM_WINDOWS
	::Sleep( static_cast<DWORD>(time) );
#else
	#error "Implementation is missing"
#endif
}

//-----------------------------------//

} // end namespace