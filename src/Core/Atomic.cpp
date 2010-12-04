/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "Atomic.h"

#ifndef VAPOR_PLATFORM_WINDOWS
	#error "Implement atomics support in UNIX"
#endif

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
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
	return InterlockedExchange(&value, val);
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
#else
	return ++value;
#endif
}

//-----------------------------------//

uint Atomic::dec()
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return InterlockedDecrement(&value);
#else
	return --value;
#endif
}

//-----------------------------------//

} // end namespace