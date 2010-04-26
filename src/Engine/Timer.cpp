/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Timer.h"

using namespace vapor::log;

namespace vapor {

bool Timer::checked = false;
ticks_t Timer::ticksPerSecond = 0;

//-----------------------------------//

Timer::Timer()
	: lastTime( 0 ), currentTime( 0 )
{
	if( !checked )
	{
		if( !checkSupport() )
		{
			Log::MessageDialog( 
				"High-resolution timers are not supported",
				LogLevel::Error );

			// TODO: use low-precision timers
		}
	}

	reset();
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

	checked = true;
	
#else
	#error "Implement me pl0x"
#endif

	return true;
}

//-----------------------------------//

void Timer::sleep( double time )
{
#if defined(VAPOR_PLATFORM_WINDOWS)
	::Sleep( static_cast<DWORD>( time ) );
#elif
	#error Implement sleeping support for your platform
#endif
}

//-----------------------------------//

} // end namespace