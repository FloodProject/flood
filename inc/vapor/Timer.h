/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

BEGIN_NAMESPACE_EXTERN

//-----------------------------------//

struct MemoryAllocator;

/**
 * Represents a timer that can be used to measure time precisely.
 * Uses high-precision timers on Windows if supported, else it will use
 * the general lower-precision timers. On POSIX platforms it'll use the
 * most high-precision timer available.
 */

CORE_API float TimerGetCurrentTimeMs();

struct Timer { int64 time; };

CORE_API Timer* TimerCreate(MemoryAllocator*);
CORE_API void   TimerDestroy(Timer*, MemoryAllocator*);
CORE_API void   TimerReset(Timer*);
CORE_API float  TimerGetElapsed(Timer*);
CORE_API void   TimerSleep( int64 time );
//-----------------------------------//

END_NAMESPACE_EXTERN