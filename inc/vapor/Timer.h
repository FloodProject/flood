/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifndef VAPOR_PLATFORM_WINDOWS
	#include <sys/time.h>
#endif

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	typedef int64 Ticks;
#else
	typedef timeval Ticks;
#endif

//-----------------------------------//

/**
 * Represents a timer that can be used to measure time precisely.
 * Uses high-precision timers on Windows if supported, else it will use
 * the general lower-precision timers. On POSIX platforms it'll use the
 * most high-precision timer available.
 */

class CORE_API Timer
{
	DECLARE_UNCOPYABLE(Timer)

public:

	Timer();

	// Resets the timer.
	float reset();

	// Gets the time since the last reset.
	float getElapsedTime();

	// Gets the current time.
	float getCurrentTime();
	
private:

	// Checks if high-resolution timers are available.
	bool checkHighResolutionTimers();

	// Stores the current time in the variable.
	void storeTime( Ticks& var );

	// Holds the current time (used for calculating the diff).
	Ticks currentTime;

	// Holds the time when the last reset happened.
	Ticks lastTime;

	// Holds the ticks per second (timer resolution).
	static Ticks ticksPerSecond;

	// Holds if we have checked for high resolution timers.
	static bool checked;

	// Holds if high resolution timers are supported.
	static bool highResolutionSupport;
};

//-----------------------------------//

} // end namespace