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

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(RESOURCE_API_DLL) && defined(RESOURCE_API_DLL_EXPORT)
	#ifdef VAPOR_PLATFORM_WINDOWS
		#define RESOURCE_API __declspec( dllexport )
	#else
		#define RESOURCE_API __attribute__ ((visibility("default")))
	#endif
#elif defined(RESOURCE_API_DLL) && defined(VAPOR_PLATFORM_WINDOWS)
	#define RESOURCE_API __declspec( dllimport )
#else
	#define RESOURCE_API
#endif