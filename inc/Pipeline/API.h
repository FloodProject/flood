/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Pipeline APIs
//---------------------------------------------------------------------//

#define ENABLE_IMPORTER_FBX
#define ENABLE_IMPORTER_MILKSHAPE

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

void PipelineInit();
void PipelineCleanup();

#include "Core/Memory.h"
