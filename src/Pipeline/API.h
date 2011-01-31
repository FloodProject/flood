/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Platform headers
//---------------------------------------------------------------------//

#include "Core/API.h"
#include "Resources/API.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(PIPELINE_API_DLL) && defined(PIPELINE_API_DLL_EXPORT)
	#ifdef VAPOR_PLATFORM_WINDOWS
		#define PIPELINE_API __declspec( dllexport )
	#else
		#define PIPELINE_API __attribute__ ((visibility("default")))
	#endif
#elif defined(PIPELINE_API_DLL) && defined(VAPOR_PLATFORM_WINDOWS)
	#define PIPELINE_API __declspec( dllimport )
#else
	#define PIPELINE_API
#endif