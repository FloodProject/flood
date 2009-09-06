/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/Timer.h"

namespace vapor {

//-----------------------------------//

Timer::Timer()
{
	if( !checkSupport() )
	{
		log::Log::MessageDialog( 
			"High-resolution timers are not supported",
			log::LogLevel::Error );

		// TODO: exit
	}

	reset();
}

//-----------------------------------//

Timer::~Timer()
{

}

//-----------------------------------//

double Timer::getCurrentTime()
{
#ifdef VAPOR_PLATFORM_WINDOWS

	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );

#else
	#error "Implement me pl0x"
#endif

	return (double) currentTime;
}

//-----------------------------------//

double Timer::getDeltaTime()
{
	getCurrentTime();	

	ticks_t diff = ( currentTime - lastTime ); /// ticksPerSecond;

	return double( diff ) / double( ticksPerSecond );
}

//-----------------------------------//

void Timer::reset()
{
#ifdef VAPOR_PLATFORM_WINDOWS

	QueryPerformanceCounter( (LARGE_INTEGER *) &lastTime );

#else
	#error "Implement me pl0x"
#endif
}

//-----------------------------------//

bool Timer::checkSupport()
{
#ifdef VAPOR_PLATFORM_WINDOWS
	
	if( !QueryPerformanceFrequency( (LARGE_INTEGER *) &ticksPerSecond ) )
	{
		return false;
	}
	
#else
	#error "Implement me pl0x"
#endif

	return true;
}

//-----------------------------------//

} // end namespace