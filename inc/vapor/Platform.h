/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

//-------------------------------------------------------------------------//
// Compile-time options
//-------------------------------------------------------------------------//

#include "vapor/CompileOptions.h"

//-------------------------------------------------------------------------//
// Platform detection
//-------------------------------------------------------------------------//

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	#define VAPOR_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX)
	#define VAPOR_PLATFORM_MACOSX
#else
	#define VAPOR_PLATFORM_LINUX
#elif
	#error "Unknown platform (not currently supported by the engine)"
#endif

//-------------------------------------------------------------------------//
// Platform headers
//-------------------------------------------------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	// include the main Windows headers
	#define WIN32_LEAN_AND_MEAN 
	#include <Windows.h>	
#endif

//-------------------------------------------------------------------------//
// Portable types
//-------------------------------------------------------------------------//

#include "vapor/Types.h"

//-------------------------------------------------------------------------//
// Memory stuff
//-------------------------------------------------------------------------//

#include "vapor/Memory.h"

//-------------------------------------------------------------------------//
// Logging. Use and abuse it.
//-------------------------------------------------------------------------//

#include "vapor/Log.h"

using vapor::log::info;
using vapor::log::warn;
using vapor::log::error;
