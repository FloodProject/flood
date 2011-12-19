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
#include "Graphics/API.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_ENGINE_DLL) && defined(API_ENGINE_DLL_EXPORT)
	#define API_ENGINE API_EXPORT
#elif defined(API_ENGINE_DLL)
	#define API_ENGINE API_IMPORT
#else
	#define API_ENGINE
#endif

#define API_SCENE API_ENGINE
#define API_AUDIO API_ENGINE
#define API_INPUT API_ENGINE

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_ENGINE_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_ENGINE_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Logging. Use and abuse it.
//---------------------------------------------------------------------//

#include "Core/Log.h"

#define DECLARE_FRIENDS(Type) \
	friend API_ENGINE Type* Type##Create(Allocator* alloc); \
	friend API_ENGINE void* Type##CreateFactory(Allocator* alloc);
