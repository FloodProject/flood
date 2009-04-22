/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

//-------------------------------------------------------------------------//
// Platform detection
//-------------------------------------------------------------------------//

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	// Microsoft Windows
	#define VAPOR_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX)
	// Apple MacOS X
	#define VAPOR_PLATFORM_MACOSX
#else
	// Linux
	#define VAPOR_PLATFORM_LINUX
#elif
	#error "Unknown platform (not currently supported by the engine)"
#endif

//-------------------------------------------------------------------------//
// Platform main headers
//-------------------------------------------------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	// include the main Windows headers
	#define WIN32_LEAN_AND_MEAN 
	#include <Windows.h>	
#endif

