/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Base API
//---------------------------------------------------------------------//

#include "Core/API.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_PLATFORM_DLL) && defined(API_PLATFORM_DLL_EXPORT)
	#define API_PLATFORM API_EXPORT
#elif defined(API_PLATFORM_DLL)
	#define API_PLATFORM API_IMPORT
#else
	#define API_PLATFORM
#endif