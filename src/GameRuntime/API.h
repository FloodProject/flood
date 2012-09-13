/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Runtime/API.h"

#if defined(API_GAMERUNTIME_DLL) && defined(API_GAMERUNTIME_DLL_EXPORT)
	#define API_GAMERUNTIME API_EXPORT
#elif defined(API_GAMERUNTIME_DLL)
	#define API_GAMERUNTIME API_IMPORT
#else
	#define API_GAMERUNTIME
#endif

#define NAMESPACE_GAMERUNTIME_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_GAMERUNTIME_END NAMESPACE_CORE_END