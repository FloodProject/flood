/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//-------------------------------------------------------------------------//
// Engine information
//-------------------------------------------------------------------------//

#define VAPOR_ENGINE_VERSION "vapor3D-SVN"

//-------------------------------------------------------------------------//
// API building information
//-------------------------------------------------------------------------//

// Enable this when building target is a DLL
//#define VAPOR_API_DLL

// Enables visual styled controls on Windows
//#define VAPOR_USE_COMMON_CONTROLS

//-------------------------------------------------------------------------//
// Memory stuff
//-------------------------------------------------------------------------//
 
#define VAPOR_MEMORY_TR1_VENDOR
//#define VAPOR_MEMORY_TR1_BOOST
//#define VAPOR_MEMORY_LEAK_DETECTOR

#define VAPOR_MEMORY_SHARED_PTR
#define VAPOR_MEMORY_INTRUSIVE_PTR

//-------------------------------------------------------------------------//
// Threading stuff
//-------------------------------------------------------------------------//

#define VAPOR_THREADING

#define VAPOR_THREADING_BOOST
//#define VAPOR_THREADING_STD

//#if defined(VAPOR_THREADING)
//	#if !defined(VAPOR_THREADING_BOOST) || !defined(VAPOR_THREADING_STD)
//		#error Choose a threading implementation
//	#endif
//
//	#if defined(VAPOR_THREADING_BOOST) && defined(VAPOR_THREADING_STD)
//		#error Only one threading implementation allowed
//	#endif
//#endif

//-------------------------------------------------------------------------//
// VFS providers
//-------------------------------------------------------------------------//

#define VAPOR_VFS_PHYSFS

//-------------------------------------------------------------------------//
// Windowing APIs
//-------------------------------------------------------------------------//

// Set this to either version '12' (1.2) or '13' (1.3)
//#define VAPOR_WINDOWING_SDL 12
#define VAPOR_WINDOWING_SFML
//#define VAPOR_WINDOWING_WIN32

//-------------------------------------------------------------------------//
// Graphics APIs
//-------------------------------------------------------------------------//

#define VAPOR_RENDERER_OPENGL
//#define VAPOR_RENDERER_DIRECTX
//#define VAPOR_RENDERER_SOFTWARE
//#define VAPOR_RENDERER_NULL

#define VAPOR_SHADER_GLSL

//-------------------------------------------------------------------------//
// Audio APIs
//-------------------------------------------------------------------------//

//#define VAPOR_AUDIO_OPENAL

//-------------------------------------------------------------------------//
// Input APIs
//-------------------------------------------------------------------------//

#define VAPOR_INPUT_SFML

//-------------------------------------------------------------------------//
// Audio codecs
//-------------------------------------------------------------------------//

#define VAPOR_AUDIO_OGG

//-------------------------------------------------------------------------//
// Image codecs
//-------------------------------------------------------------------------//

//#define VAPOR_IMAGE_DEVIL
//#define VAPOR_IMAGE_PICOPNG
#define VAPOR_IMAGE_STB

//-------------------------------------------------------------------------//
// Mesh formats
//-------------------------------------------------------------------------//

#define VAPOR_MESH_MILKSHAPE3D

//-------------------------------------------------------------------------//
// Font providers
//-------------------------------------------------------------------------//

#define VAPOR_FONT_FREETYPE2
#define VAPOR_FONT_BITMAP

//-------------------------------------------------------------------------//
// Scripting languages
//-------------------------------------------------------------------------//

//#define VAPOR_SCRIPTING_LUA