-- 
-- Keeps the build settings. Each developer should keep a local modified
-- version of this file so there are no conflicts between the different
-- developer build configurations.
--

config =
{
--
-- Core
--

	-- Enables custom STL containers in the core.
	STL_RDE = false,
	
	--  Enables intrusive reference-counting pointers.
	MEMORY_INTRUSIVE_PTR = true,

	-- Enables the VLD memory leak detector.
	MEMORY_LEAK_DETECTOR = false,

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

	-- Enables serialization support.
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

	-- Enables loading of Mono scripts.
	SCRIPTING_MONO = true,

	-- Enables physics support via Bullet.
	PHYSICS_BULLET = true,

	-- Enables loading of PNG images via picopng.
	IMAGE_PICOPNG = false,

	-- Enables loading of images via the STB library.
	IMAGE_STB = true,

--
-- Graphics
--	

	-- Enables the OpenGL renderer.
	RENDERER_OPENGL = true,

	-- Enables support for GLSL shaders.
	RENDERER_OPENGL_GLSL = true,

	-- Enables the Direct3D renderer.
	RENDERER_DIRECT3D = false,

	-- Enables loading of OpenGL GLSL shaders.
	RENDERER_OPENGL_GLSL = true,

--
-- Audio
--

	-- Enables loading of audio OGG resources.
	AUDIO_OGG = true,

	-- Enables audio playback via OpenAL.
	AUDIO_OPENAL = true,

--
-- GUI
--

	-- Enables the GUI extensions
	GUI = true,

	-- Enables bitmap font handling.
	FONT_BITMAP = true,

	-- Enables font handling using FreeType.
	FONT_FREETYPE2 = false,

	-- Enables font handling using DirectWrite.
	FONT_DIRECTWRITE = false,	

--
-- Pipeline
--

	-- Enables the MS3D mesh importer.
	IMPORTER_MILKSHAPE = true,

	-- Enables the FBX mesh importer.
	IMPORTER_FBX = false,

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
	PLUGIN_MONO = false,

	-- Enables the server plugin.
	PLUGIN_SERVER = false,

	-- Disables the networking integration.
	NO_NETWORK = true,
}