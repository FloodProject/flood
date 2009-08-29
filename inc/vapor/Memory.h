/************************************************************************
*
* vaporEngine (2008-2009)
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
	#elif defined(VAPOR_COMPILER_GCC)
		#include <tr1/memory>
	#endif
#elif defined(VAPOR_MEMORY_TR1_BOOST)
	#include <boost/tr1/memory.hpp>
#endif

using std::tr1::shared_ptr;

//-------------------------------------------------------------------------//
// Memory leaks analyzer
//-------------------------------------------------------------------------//

#ifdef VAPOR_MEMORY_LEAK_DETECTOR

#ifdef VAPOR_PLATFORM_WINDOWS
	#ifdef _DEBUG
		// Visual Leak Detector
		// (http://dmoulding.googlepages.com/vld)
		#include <vld.h>

		//#define _CRTDBG_MAP_ALLOC
		//#include <stdlib.h>
		//#include <crtdbg.h>
	#endif
#endif

#endif