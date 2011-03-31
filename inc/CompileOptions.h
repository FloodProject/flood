/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// Core APIs
//---------------------------------------------------------------------//

#define		VAPOR_MEMORY_TR1_VENDOR
#define		VAPOR_MEMORY_SHARED_PTR
#define		VAPOR_MEMORY_INTRUSIVE_PTR
#define		VAPOR_MEMORY_LEAK_DETECTOR
//#define	VAPOR_THREADING
//#define	VAPOR_DYNAMIC_LIBRARY
#define		VAPOR_ARCHIVE_ZIP
#define		VAPOR_ARCHIVE_DIR
//#define	VAPOR_NETWORKING_ZMQ
//#define	VAPOR_NETWORKING_CURL
//#define		VAPOR_VFS_PHYSFS
#define		VAPOR_VFS_FILEWATCHER
#define		VAPOR_SERIALIZATION_JSON
#define		VAPOR_SERIALIZATION_BINARY

//---------------------------------------------------------------------//
// Engine APIs
//---------------------------------------------------------------------//

// Enables visual styled controls on Windows
//#define	VAPOR_USE_COMMON_CONTROLS

//#define	VAPOR_WINDOWING_SFML
//#define	VAPOR_WINDOWING_WIN32
//#define	VAPOR_INPUT_SFML

#define		VAPOR_RENDERER_OPENGL
//#define	VAPOR_RENDERER_DIRECTX
#define		VAPOR_SHADER_GLSL

#define		VAPOR_AUDIO_OPENAL
#define		VAPOR_PHYSICS_BULLET

//---------------------------------------------------------------------//
// Resource APIs
//---------------------------------------------------------------------//

#define		VAPOR_AUDIO_OGG
//#define	VAPOR_IMAGE_DEVIL
//#define	VAPOR_IMAGE_PICOPNG
#define		VAPOR_IMAGE_STB
#define		VAPOR_MESH_MILKSHAPE3D
//#define	VAPOR_FONT_FREETYPE2
#define		VAPOR_FONT_BITMAP
#define		VAPOR_SCRIPTING_LUA
#define		VAPOR_SCRIPTING_MONO
