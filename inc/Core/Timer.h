/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Represents a timer that can be used to measure time precisely.
 * Uses high-precision timers on Windows if supported, else it will use
 * the general lower-precision timers. On POSIX platforms it'll use the
 * most high-precision timer available.
 */
class API_CORE Timer
{
public:

	Timer();

	/**
	 * Reset timer.
	 */
	void reset();
	
	/**
	 * Get elapsed time since timer creation or last reset.
	 */
	float getElapsed();

	/**
	 * Get current time in miliseconds.
	 */
	float getCurrentTimeMs();

	int64 time; //!< time of timer creation
};

//-----------------------------------//

NAMESPACE_CORE_END