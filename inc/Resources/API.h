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
// Resource APIs
//---------------------------------------------------------------------//

#define     ENABLE_AUDIO_OGG
//#define   ENABLE_IMAGE_DEVIL
//#define   ENABLE_IMAGE_PICOPNG
#define     ENABLE_IMAGE_STB
#define     ENABLE_SHADER_GLSL
#define     ENABLE_MESH_MILKSHAPE3D
//#define   ENABLE_FONT_FREETYPE2
#define     ENABLE_FONT_BITMAP
#define     ENABLE_SCRIPTING_LUA
#define     ENABLE_SCRIPTING_MONO
#define     ENABLE_THREADED_LOADING

#define     ENABLE_RESOURCE_INDEXER

//---------------------------------------------------------------------//
// Custom Allocators
//---------------------------------------------------------------------//

NAMESPACE_BEGIN

Allocator* GetResourcesAllocator();

NAMESPACE_END