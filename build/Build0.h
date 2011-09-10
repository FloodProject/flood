/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

/** DO NOT PUT YOUR LOCAL COPY OF THIS UNDER VERSION CONTROL **/

/**
 * Keeps the build settings. Each developer should keep a local modified
 * version of this file so there are no conflicts between the different
 * developer build configurations.
 */

//-----------------------------------//
// Core

// Enables custom STL containers in the core.
//#define   ENABLE_STL_RDE

// Enables intrusive reference-counting pointers.
#define     ENABLE_MEMORY_INTRUSIVE_PTR

// Enables the VLD memory leak detector.
#define     ENABLE_MEMORY_LEAK_DETECTOR

// Enables the support for dynamic libraries loading.
//#define   ENABLE_DYNAMIC_LIBRARY

// Enables ZIP support for archives.
#define     ENABLE_ARCHIVE_ZIP

// Enables OS directory support for archives.
#define     ENABLE_ARCHIVE_DIR

// Enables the VFS support for archives.
#define     ENABLE_ARCHIVE_VIRTUAL

// Enables support for monitoring archives.
//#define   ENABLE_VFS_FILEWATCHER

// Enables the stack walking debug code.
#define     ENABLE_STACK_WALKER

// Enables serializtion support.
#define     ENABLE_SERIALIZATION

// Enables the JSON serializer.
#define     ENABLE_SERIALIZATION_JSON

// Enables the binary serializer.
#define     ENABLE_SERIALIZATION_BINARY

// Enables HTTP support via libCURL.
#define     ENABLE_NETWORKING_CURL

// Enables lightweight HTTP server support.
#define     ENABLE_HTTP_SERVER

//-----------------------------------//
// Resources

// Enables loading of audio OGG resources.
#define     ENABLE_AUDIO_OGG

// Enables loading of PNG images via picopng.
//#define   ENABLE_IMAGE_PICOPNG

// Enables loading of images via the STB library.
#define     ENABLE_IMAGE_STB

// Enables loading of OpenGL GLSL shaders.
#define     ENABLE_RENDERER_OPENGL_GLSL

// Enables loading of fonts via FreeType.
//#define   ENABLE_FONT_FREETYPE2

// Enables loading of bitmap based fonts.
#define     ENABLE_FONT_BITMAP

// Enables loading of Lua scripts.
#define     ENABLE_SCRIPTING_LUA

// Enables loading of Mono scripts.
#define     ENABLE_SCRIPTING_MONO

// Enables threaded loading of resources.
#define     ENABLE_THREADED_LOADING

// Enables the indexing of resources.
#define     ENABLE_RESOURCE_INDEXER

//-----------------------------------//
// Engine

// Enables the window support via raw Win32 API.
#define     ENABLE_WINDOW_WIN32

// Enables window support using the SFML library.
//#define   ENABLE_WINDOW_SFML

// Enables input support using the SFML library.
//#define   ENABLE_INPUT_SFML

// Enables the OpenGL renderer.
#define     ENABLE_RENDERER_OPENGL

// Enables support for GLSL shaders.
#define     ENABLE_RENDERER_OPENGL_GLSL

// Enables the Direct3D renderer.
//#define   ENABLE_RENDERER_DIRECT3D

// Enables audio playback via OpenAL.
#define     ENABLE_AUDIO_OPENAL

// Enables physics support via Bullet.
#define     ENABLE_PHYSICS_BULLET

//-----------------------------------//
// Editor

// Enables the gizmo manipulators plugin.
#define     ENABLE_PLUGIN_GIZMO

// Enables the terrain editing plugin.
#define     ENABLE_PLUGIN_TERRAIN

// Enables the property plugin.
#define     ENABLE_PLUGIN_PROPERTY

// Enables the resource browser.
#define     ENABLE_RESOURCE_BROWSER

// Enables the Mono scripting plugin.
//#define ENABLE_PLUGIN_MONO

// Enables the server plugin.
//#define ENABLE_PLUGIN_SERVER

// Disables the networking integration.
#define NO_NETWORK