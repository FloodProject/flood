/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Platform detection
//---------------------------------------------------------------------//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	#define VAPOR_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(MACOSX)
	#define VAPOR_PLATFORM_MACOSX
#elif defined(__LINUX) || defined(__LINUX__) || defined(LINUX)
	#define VAPOR_PLATFORM_LINUX
#else
	#error "Unknown platform (not currently supported by the engine)"
#endif

//---------------------------------------------------------------------//
// Compiler detection
//---------------------------------------------------------------------//

#ifdef _MSC_VER // Microsoft Visual C++ Compiler
	#if _MSC_VER == 1600
		#define VAPOR_COMPILER_MSVC_2010 0x1600
		#define VAPOR_COMPILER_MSVC	VAPOR_COMPILER_MSVC_2010
	#elif _MSC_VER == 1500
		#define VAPOR_COMPILER_MSVC_2008 0x1500
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

#if defined( DEBUG ) || defined( _DEBUG ) || defined( ___DEBUG )
	#define VAPOR_DEBUG
#elif !defined( VAPOR_DEBUG ) && !defined( NDEBUG )
	#define NDEBUG
#elif defined( VAPOR_DEBUG ) && defined( NDEBUG )
	#error
#endif

//---------------------------------------------------------------------//
// Memory leaks analyzer
//---------------------------------------------------------------------//

#ifdef VAPOR_MEMORY_LEAK_DETECTOR
	#if defined(VAPOR_PLATFORM_WINDOWS) && defined(VAPOR_DEBUG)
		// Visual Leak Detector
		// (http://dmoulding.googlepages.com/vld)
		#include <vld.h>
	#endif
#endif

//---------------------------------------------------------------------//
// Alignment control
//---------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#define VAPOR_ALIGN_BEGIN( alignment ) __declspec (align( alignment ))
	#define VAPOR_ALIGN_END( alignment )
#elif defined(VAPOR_COMPILER_GCC)
	#define VAPOR_ALIGN_BEGIN( alignment )
	#define VAPOR_ALIGN_END( alignment ) __attribute__ ((aligned( alignment )))
#else
	#error "Implement alignment support for your compiler"
#endif

#if defined(VAPOR_COMPILER_MSVC)
	#define VAPOR_INLINE __forceinline
#elif defined(VAPOR_COMPILER_GCC)
	#define VAPOR_INLINE __attribute__((always_inline))
#else
	#error "Implement inlining support for your compiler"
#endif