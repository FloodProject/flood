/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Atomic.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#elif defined VAPOR_PLATFORM_MACOSX
	#include <libkern/OSAtomic.h>
#else
	#error "Implement atomics support in UNIX"
#endif

namespace vapor {

//-----------------------------------//

Atomic::Atomic(uint val)
{
	set(val);
}

//-----------------------------------//

Atomic::~Atomic()
{ }

//-----------------------------------//

uint Atomic::set(uint val)
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return InterlockedCompareExchange((volatile long*)&val, value, val);
#elif defined VAPOR_PLATFORM_MACOSX
	return (uint)OSAtomicCompareAndSwapIntBarrier((int)value, (int)val, (int* volatile)&val);
#else
	value = val;
#endif
}

//-----------------------------------//

uint Atomic::get() const
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return value;
#else
	return value;
#endif
}

//-----------------------------------//

uint Atomic::add(uint val)
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return InterlockedExchangeAdd(&value, val);
#elif defined VAPOR_PLATFORM_MACOSX
	return OSAtomicAdd32Barrier(val, (int*)&value);
#else
	value += val;
	return value;
#endif
}

//-----------------------------------//

uint Atomic::sub(uint val)
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return InterlockedExchangeAdd(&value, -(int)val);
#elif defined VAPOR_PLATFORM_MACOSX
	return OSAtomicAdd32Barrier(-(int)val, (int*)&value);
#else
	value -= val;
	return value;
#endif
}

//-----------------------------------//

uint Atomic::inc()
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return InterlockedIncrement(&value);
#elif defined VAPOR_PLATFORM_MACOSX
	return OSAtomicIncrement32Barrier((volatile int*)&value);
#else
	return ++value;
#endif
}

//-----------------------------------//

uint Atomic::dec()
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return InterlockedDecrement(&value);
#elif defined VAPOR_PLATFORM_MACOSX
	return OSAtomicDecrement32Barrier((volatile int*)&value);
#else
	return --value;
#endif
}

//-----------------------------------//

} // end namespace