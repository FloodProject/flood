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
#include "Engine/API.h"
#include "Resources/API.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_PIPELINE_DLL) && defined(API_PIPELINE_DLL_EXPORT)
	#define API_PIPELINE API_EXPORT
#elif defined(API_PIPELINE_DLL)
	#define API_PIPELINE API_IMPORT
#else
	#define API_PIPELINE
#endif

#define NAMESPACE_PIPELINE_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_PIPELINE_END NAMESPACE_CORE_END

#include "Core/Memory.h"

NAMESPACE_PIPELINE_BEGIN

// Initializes the default resource processors.
API_PIPELINE void PipelineInit();

// Cleans the resource processors.
API_PIPELINE void PipelineCleanup();

NAMESPACE_PIPELINE_END