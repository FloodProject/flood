/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Compile-time options
//---------------------------------------------------------------------//

#include "vapor/CompileOptions.h"

//---------------------------------------------------------------------//
// Platform detection
//---------------------------------------------------------------------//

#if defined( _WIN32 ) || defined( _WIN64 ) || defined( WIN32 ) || defined( WIN64 )
	#define VAPOR_PLATFORM_WINDOWS
#elif defined( __APPLE__ ) || defined( MACOSX )
	#define VAPOR_PLATFORM_MACOSX
#elif defined(__LINUX) || defined(__LINUX__) || defined(LINUX)
	#define VAPOR_PLATFORM_LINUX
#else
	#error "Unknown platform (not currently supported by the engine)"
#endif

//---------------------------------------------------------------------//
// Compiler detection
//---------------------------------------------------------------------//

#define VAPOR_COMPILER_MSVC_2008 0x1500
#define VAPOR_COMPILER_MSVC_2010 0x1600

#ifdef _MSC_VER
	// Microsoft Visual C++
	#if _MSC_VER == 1600
		#define VAPOR_COMPILER_MSVC	VAPOR_COMPILER_MSVC_2010
	#elif _MSC_VER == 1500
		#define VAPOR_COMPILER_MSVC	VAPOR_COMPILER_MSVC_2008
	#else
		#error "Unknown Visual C++ compiler version"
	#endif
#elif defined(__GNUG__)
	// GCC
	#define VAPOR_COMPILER_GCC
#else
	#error "Unknown compiler (not currently supported by the engine)"
#endif

//---------------------------------------------------------------------//
// Build mode
//---------------------------------------------------------------------//

#if defined( _DEBUG ) || defined( ___DEBUG ) || defined( DEBUG )
	#define VAPOR_DEBUG
#endif

#if !defined( VAPOR_DEBUG ) && !defined( NDEBUG )
	#define NDEBUG
#endif

#if defined( VAPOR_DEBUG ) && defined( NDEBUG )
	#error
#endif

//---------------------------------------------------------------------//
// Platform headers
//---------------------------------------------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	// include the main Windows headers
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

//---------------------------------------------------------------------//
// Portable types
//---------------------------------------------------------------------//

#include "vapor/Types.h"

//---------------------------------------------------------------------//
// Memory stuff
//---------------------------------------------------------------------//

#include "vapor/Memory.h"

//---------------------------------------------------------------------//
// Language features (C++)
//---------------------------------------------------------------------//

#include "vapor/Language.h"