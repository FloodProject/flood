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

#if defined(VAPOR_MEMORY_TR1_BOOST)
	#include <boost/tr1/memory.hpp>
#elif defined(VAPOR_MEMORY_TR1_VENDOR)
	#include <memory>
#endif

using std::auto_ptr;
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::mem_fn;
using std::tr1::bind;

//-------------------------------------------------------------------------//
// Memory leaks analyzer
//-------------------------------------------------------------------------//

#ifdef VAPOR_MEMORY_LEAK_DETECTOR

#ifdef VAPOR_PLATFORM_WINDOWS
	// taken from 3Demon
	#ifdef _DEBUG
		// Visual Leak Detector 
		// (http://dmoulding.googlepages.com/vld)
		//#include <vld.h>

		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
	#endif
#endif

#endif