/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/Timer.h"

using namespace vapor::log;

namespace vapor {

//-----------------------------------//

Timer::Timer()
	: lastTime( 0 ), currentTime( 0 ), ticksPerSecond( 0 )
{
	if( !checkSupport() )
	{
		Log::MessageDialog( 
			"High-resolution timers are not supported",
			LogLevel::Error );

		// TODO: use low-precision timers
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

	QueryPerformanceCounter( reinterpret_cast< LARGE_INTEGER* >( &currentTime ) );

#else
	#error "Implement me pl0x"
#endif

	return static_cast< double >( currentTime );
}

//-----------------------------------//

double Timer::getElapsedTime()
{
	getCurrentTime();	

	ticks_t diff = ( currentTime - lastTime );

	return static_cast< double >( diff ) / static_cast< double >( ticksPerSecond );
}

//-----------------------------------//

void Timer::reset()
{
#ifdef VAPOR_PLATFORM_WINDOWS

	QueryPerformanceCounter( reinterpret_cast< LARGE_INTEGER* >( &lastTime ) );

#else
	#error "Implement me pl0x"
#endif
}

//-----------------------------------//

bool Timer::checkSupport()
{
#ifdef VAPOR_PLATFORM_WINDOWS
	
	if( !QueryPerformanceFrequency( reinterpret_cast< LARGE_INTEGER* >( &ticksPerSecond ) ) )
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