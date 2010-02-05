/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PLATFORM_WINDOWS
	#define ticks_t int64
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
	~Timer ();

	// Gets the current time.
	double getCurrentTime();

	// Gets the time since the last reset.
	double getElapsedTime();

	// Resets the timer.
	void reset();

	static void sleep( double time );
	
private:

	// Checks if high-resolution timers are available.
	bool checkSupport();

	// Holds the current time (used for calculating the diff).
	ticks_t currentTime;

	// Holds the time when the last reset happened.
	ticks_t lastTime;

	// Holds the ticks per second (timer resolution).
	ticks_t ticksPerSecond;
};

//-----------------------------------//

} // end namespace
