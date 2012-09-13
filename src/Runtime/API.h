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

#if defined(API_RUNTIME_DLL) && defined(API_RUNTIME_DLL_EXPORT)
	#define API_RUNTIME API_EXPORT
#elif defined(API_RUNTIME_DLL)
	#define API_RUNTIME API_IMPORT
#else
	#define API_RUNTIME
#endif

#define NAMESPACE_RUNTIME_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_RUNTIME_END   NAMESPACE_CORE_END

API_RUNTIME void RuntimeInit();
API_RUNTIME void RuntimeCleanup();

