-- 
-- Keeps the build settings. Each developer should keep a local modified
-- version of this file so there are no conflicts between the different
-- developer build configurations.
--

config =
{
	-- Enables custom STL containers in the core.
	STL_RDE = false,
	
	--  Enables intrusive reference-counting pointers.
	MEMORY_INTRUSIVE_PTR = true,

	-- Enables the VLD memory leak detector.
	MEMORY_LEAK_DETECTOR = true,

	-- Enables the support for dynamic libraries loading.
	DYNAMIC_LIBRARY = false,

	-- Enables ZIP support for archives.
	ARCHIVE_ZIP = true,

	-- Enables OS directory support for archives.
	ARCHIVE_DIR = true,

	-- Enables the VFS support for archives.
	ARCHIVE_VIRTUAL = true,

	-- Enables support for monitoring archives.
	VFS_FILEWATCHER = true,

	-- Enables the stack walking debug code.
	STACK_WALKER = true,

	-- Enables serializtion support.
	SERIALIZATION = true,

	-- Enables the JSON serializer.
	SERIALIZATION_JSON = true,

	-- Enables the binary serializer.
	SERIALIZATION_BINARY = true,

	-- Enables HTTP support via libCURL.
	NETWORKING_CURL = true,

	-- Enables lightweight HTTP server support.
	HTTP_SERVER = true,

--
-- Resources
--

	-- Enables loading of audio OGG resources.
	AUDIO_OGG = true,

	-- Enables loading of PNG images via picopng.
	IMAGE_PICOPNG = false,

	-- Enables loading of images via the STB library.
	IMAGE_STB = true,

	-- Enables loading of OpenGL GLSL shaders.
	RENDERER_OPENGL_GLSL = true,

	-- Enables loading of fonts via FreeType.
	FONT_FREETYPE2 = false,

	-- Enables loading of bitmap based fonts.
	FONT_BITMAP = true,

	-- Enables loading of Lua scripts.
	SCRIPTING_LUA = true,

	-- Enables loading of Mono scripts.
	SCRIPTING_MONO = true,

	-- Enables threaded loading of resources.
	THREADED_LOADING = true,

	-- Enables the indexing of resources.
	RESOURCE_INDEXER = true,

--
-- Engine
--

	-- Enables the window support via raw Win32 API.
	WINDOW_WIN32 = true,

	-- Enables window support using the SFML library.
	WINDOW_SFML = false,

	-- Enables input support using the SFML library.
	INPUT_SFML = false,

	-- Enables the OpenGL renderer.
	RENDERER_OPENGL = true,

	-- Enables support for GLSL shaders.
	RENDERER_OPENGL_GLSL = true,

	-- Enables the Direct3D renderer.
	RENDERER_DIRECT3D = false,

	-- Enables audio playback via OpenAL.
	AUDIO_OPENAL = true,

	-- Enables physics support via Bullet.
	PHYSICS_BULLET = true,

--
-- Editor
--

	-- Enables the gizmo manipulators plugin.
	PLUGIN_GIZMO = true,

	-- Enables the terrain editing plugin.
	PLUGIN_TERRAIN = true,

	-- Enables the property plugin.
	PLUGIN_PROPERTY = true,

	-- Enables the resource browser.
	RESOURCE_BROWSER = true,

	-- Enables the Mono scripting plugin.
	PLUGIN_MONO = true,

	-- Enables the server plugin.
	PLUGIN_SERVER = false,

	-- Disables the networking integration.
	NO_NETWORK = true,	
}