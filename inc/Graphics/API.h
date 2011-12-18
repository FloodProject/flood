/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Core Includes
//---------------------------------------------------------------------//

#include "Core/API.h"
#include "Resources/API.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_GRAPHICS_DLL) && defined(API_GRAPHICS_DLL_EXPORT)
	#define API_GRAPHICS API_EXPORT
#elif defined(API_GRAPHICS_DLL)
	#define API_GRAPHICS API_IMPORT
#else
	#define API_GRAPHICS
#endif

#define API_RENDER API_GRAPHICS

#define AllocateRender AllocateHeap

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_GRAPHICS_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_GRAPHICS_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Logging. Use and abuse it.
//---------------------------------------------------------------------//

#include "Core/Log.h"
