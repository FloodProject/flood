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

//---------------------------------------------------------------------//
// Resource APIs
//---------------------------------------------------------------------//

#define		VAPOR_AUDIO_OGG
//#define	VAPOR_IMAGE_DEVIL
#define	VAPOR_IMAGE_PICOPNG
//#define		VAPOR_IMAGE_STB
#define		VAPOR_MESH_MILKSHAPE3D
//#define	VAPOR_FONT_FREETYPE2
#define		VAPOR_FONT_BITMAP
#define		VAPOR_SCRIPTING_LUA
#define		VAPOR_SCRIPTING_MONO

//---------------------------------------------------------------------//
// Custom Allocators
//---------------------------------------------------------------------//

NAMESPACE_BEGIN

Allocator* GetResourcesAllocator();

NAMESPACE_END