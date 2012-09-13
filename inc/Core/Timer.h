/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * Represents a timer that can be used to measure time precisely.
 * Uses high-precision timers on Windows if supported, else it will use
 * the general lower-precision timers. On POSIX platforms it'll use the
 * most high-precision timer available.
 */

API_CORE float TimerGetCurrentTimeMs();

struct API_CORE Timer
{
	Timer();
	int64 time;
};

API_CORE Timer* TimerCreate(Allocator*);
API_CORE void   TimerDestroy(Timer*);
API_CORE void   TimerReset(Timer*);
API_CORE float  TimerGetElapsed(Timer*);

//-----------------------------------//

NAMESPACE_EXTERN_END