/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

//-------------------------------------------------------------------------//
// Pointer wrappers
//-------------------------------------------------------------------------//

#if defined(VAPOR_MEMORY_TR1_VENDOR)
	#if defined(VAPOR_COMPILER_MSVC)
		#include <memory>
		#include <functional>
	#elif defined(VAPOR_COMPILER_GCC)
		#include <tr1/memory>
	#endif
#elif defined(VAPOR_MEMORY_TR1_BOOST)
	#include <boost/tr1/memory.hpp>
#endif

#ifdef VAPOR_COMPILER_MSVC
	#if VAPOR_COMPILER_MSVC != VAPOR_COMPILER_MSVC_2010 
		namespace std { using namespace std::tr1; }
	#endif
#endif

#if defined( VAPOR_MEMORY_SHARED_PTR )
	#define TYPEDEF_SHARED_POINTER_FROM_CLASS( class ) \
		typedef std::shared_ptr< class > class##Ptr
	#define TYPEDEF_SHARED_WEAK_POINTER_FROM_CLASS( class ) \
		typedef std::weak_ptr< class > class##WeakPtr
#endif

#if defined( VAPOR_MEMORY_INTRUSIVE_PTR )
	#include "boost/intrusive_ptr.hpp"
	#define TYPEDEF_INTRUSIVE_POINTER_FROM_CLASS( class ) \
		typedef boost::intrusive_ptr< class > class##Ptr
	using boost::static_pointer_cast;
#endif

#if !defined( VAPOR_MEMORY_SHARED_PTR ) && !defined( VAPOR_MEMORY_INTRUSIVE_PTR ) 
	#error "No shared pointer implementation found."
#endif

//-------------------------------------------------------------------------//
// Memory leaks analyzer
//-------------------------------------------------------------------------//

#ifdef VAPOR_MEMORY_LEAK_DETECTOR
	#if defined(VAPOR_PLATFORM_WINDOWS) && defined(VAPOR_DEBUG)
		// Visual Leak Detector
		// (http://dmoulding.googlepages.com/vld)
		#include <vld.h>

		//#define _CRTDBG_MAP_ALLOC
		//#include <stdlib.h>
		//#include <crtdbg.h>
	#endif
#endif

//-------------------------------------------------------------------------//
// API exports
//-------------------------------------------------------------------------//

#ifdef VAPOR_API_DLL
	#ifdef VAPOR_EXPORT
		#ifdef VAPOR_PLATFORM_WINDOWS
			// This should work for both MSVC and GCC on Windows.
			// For more details see:
			// http://gcc.gnu.org/onlinedocs/gcc-4.4.0/gcc/Function-Attributes.html
			#define VAPOR_API __declspec( dllexport )
		#else
			#define VAPOR_API __attribute__ ((visibility("default")))
		#endif
	#else
		#ifdef VAPOR_PLATFORM_WINDOWS
			#define VAPOR_API __declspec( dllimport )
		#else
			#define VAPOR_API
		#endif
	#endif
#else
	#define VAPOR_API
#endif

//-------------------------------------------------------------------------//
// Alignment control
//-------------------------------------------------------------------------//

#ifdef VAPOR_COMPILER_MSVC
	#define VAPOR_ALIGN_BEGIN( alignment ) __declspec (align( alignment ))
	#define VAPOR_ALIGN_END( alignment )
#elif defined(VAPOR_COMPILER_GCC)
	#define VAPOR_ALIGN_BEGIN( alignment )
	#define VAPOR_ALIGN_END( alignment ) __attribute__ ((aligned( alignment )))
#else
	#error "Alignment control for your platform is not currently supported"
#endif