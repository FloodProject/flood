/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Platform headers
//---------------------------------------------------------------------//

#include "Core/API.h"
#include "Resources/API.h"
#include "Engine/API.h"

//---------------------------------------------------------------------//
// Runtime APIs
//---------------------------------------------------------------------//

#if defined(FLUSH_API_DLL) && defined(FLUSH_API_DLL_EXPORT)
	#define FLUSH_API API_EXPORT
#elif defined(FLUSH_API_DLL)
	#define FLUSH_API API_IMPORT
#else
	#define FLUSH_API
#endif

#define FLUSH_NAMESPACE_BEGIN NAMESPACE_CORE_BEGIN
#define FLUSH_NAMESPACE_END   NAMESPACE_CORE_END

FLUSH_API void RuntimeInit();
FLUSH_API void RuntimeCleanup();

