/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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

#define AllocateGraphics AllocateHeap

API_GRAPHICS Allocator* GetRenderAllocator();
API_GRAPHICS Allocator* GetFrameAllocator();

API_GRAPHICS void GraphicsInitialize();
API_GRAPHICS void GraphicsDeinitialize();

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_GRAPHICS_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_GRAPHICS_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Logging. Use and abuse it.
//---------------------------------------------------------------------//

#include "Core/Log.h"
