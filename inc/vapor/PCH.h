/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Core Includes
//---------------------------------------------------------------------//

#include "Core.h"

//---------------------------------------------------------------------//
// Compile-time options
//---------------------------------------------------------------------//

#include "CompileOptions.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(VAPOR_API_DLL) && defined(VAPOR_API_DLL_EXPORT)
	#ifdef VAPOR_PLATFORM_WINDOWS
		#define VAPOR_API __declspec( dllexport )
	#else
		#define VAPOR_API __attribute__ ((visibility("default")))
	#endif
#elif defined(VAPOR_API_DLL) && defined(VAPOR_PLATFORM_WINDOWS)
	#define VAPOR_API __declspec( dllimport )
#else
	#define VAPOR_API
#endif

//---------------------------------------------------------------------//
// C++ foreach construct
//---------------------------------------------------------------------//

//#ifdef VAPOR_COMPILER_MSVC
//	#pragma warning( push )
//	#pragma warning( disable : 4512 4503 )
//	#include <boost/foreach.hpp>
//	#pragma warning( pop )
//#else
//	#include <boost/foreach.hpp>
//#endif
//
//#define foreach         BOOST_FOREACH
//#define reverse_foreach BOOST_REVERSE_FOREACH

//---------------------------------------------------------------------//
// Pointer wrappers
//---------------------------------------------------------------------//

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

#define TYPEDEF_SHARED_POINTER_FROM_TYPE( class ) \
	typedef std::shared_ptr< class > class##Ptr;

#define TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( class ) \
		typedef std::weak_ptr< class > class##WeakPtr;

//---------------------------------------------------------------------//
// Logging. Use and abuse it.
//---------------------------------------------------------------------//

#include "Log.h"