/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define ticks_t int64
#endif

namespace vapor {

//-----------------------------------//

/**
 * Use this to get timing information.
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
