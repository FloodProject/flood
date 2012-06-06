/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Base API
//---------------------------------------------------------------------//

#include "Core/API.h"
#include "Core/Reflection.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_RESOURCE_DLL) && defined(API_RESOURCE_DLL_EXPORT)
	#define API_RESOURCE API_EXPORT
#elif defined(API_RESOURCE_DLL)
	#define API_RESOURCE API_IMPORT
#else
	#define API_RESOURCE
#endif

#define NAMESPACE_RESOURCES_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_RESOURCES_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Custom Allocators
//---------------------------------------------------------------------//

NAMESPACE_RESOURCES_BEGIN

Allocator* GetResourcesAllocator();

NAMESPACE_RESOURCES_END