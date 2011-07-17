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
// Engine APIs
//---------------------------------------------------------------------//

// Enables visual styled controls on Windows
//#define	VAPOR_USE_COMMON_CONTROLS
#define		VAPOR_THREADING

//#define	VAPOR_WINDOWING_SFML
#define	VAPOR_WINDOWING_WIN32
//#define	VAPOR_INPUT_SFML

#define		VAPOR_RENDERER_OPENGL
//#define	VAPOR_RENDERER_DIRECTX
#define		ENABLE_SHADER_GLSL

#define		VAPOR_AUDIO_OPENAL
//#define		VAPOR_PHYSICS_BULLET

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

#define API_RENDER API_ENGINE
#define API_SCENE API_ENGINE
#define API_AUDIO API_ENGINE

#define VAPOR_API API_ENGINE

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_ENGINE_BEGIN namespace vapor {
#define NAMESPACE_ENGINE_END }

//---------------------------------------------------------------------//
// Pointer wrappers
//---------------------------------------------------------------------//

#if defined(ENABLE_MEMORY_TR1_VENDOR)
	#if defined(COMPILER_MSVC)
		#include <memory>
		#include <functional>
	#elif defined(COMPILER_GCC)
		#include <tr1/memory>
	#endif
#elif defined(VAPOR_MEMORY_TR1_BOOST)
	#include <boost/tr1/memory.hpp>
#endif	

#if COMPILER_MSVC != COMPILER_MSVC_2010 
	namespace std { using namespace std::tr1; }
#endif

//---------------------------------------------------------------------//
// Logging. Use and abuse it.
//---------------------------------------------------------------------//

#include "Core/Log.h"

#define DECLARE_FRIENDS(Type) \
	friend API_ENGINE Type* Type##Create(Allocator* alloc); \
	friend API_ENGINE void* Type##CreateFactory(Allocator* alloc);