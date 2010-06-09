/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PLATFORM_WINDOWS
	#define ticks_t int64
#else
	#error "Implementation is missing"
#endif

namespace vapor {

//-----------------------------------//

/**
 * Represents a timer that can be used for timing information.
 * Uses high-precision timers on Windows if supported, else it will use
 * the general lower-precision timers. On POSIX platforms it'll use the
 * most high-precision timer available.
 */

class VAPOR_API Timer : private boost::noncopyable
{
public:

	Timer();

	// Gets the current time.
	double getCurrentTime();

	// Gets the time since the last reset.
	double getElapsedTime();

	// Resets the timer.
	void reset();

	// Sleeps for a specified ammount of time.
	static void sleep( double time );
	
private:

	// Checks if high-resolution timers are available.
	bool checkHighResolutionTimers();

	// Stores the current time in the variable.
	void storeTime( ticks_t& var );

	// Holds the current time (used for calculating the diff).
	ticks_t currentTime;

	// Holds the time when the last reset happened.
	ticks_t lastTime;

	// Holds the ticks per second (timer resolution).
	static ticks_t ticksPerSecond;

	// Holds if we have checked for high resolution timers.
	static bool checked;

	// Holds if high resolution timers are supported.
	static bool highResolutionSupport;
};

//-----------------------------------//

} // end namespace
