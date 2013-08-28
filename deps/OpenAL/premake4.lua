project "OpenAL"

	SetupNativeDependencyProject()

	local version = "1.14"
	local repo = "git://repo.or.cz/openal-soft.git"
	local archive = "http://kcat.strangesoft.net/openal-releases/openal-soft-1.14.tar.bz2"
	local license = "LGPL"

	kind "StaticLib"
	language "C"
	files { "src/OpenAL32/*.c", "src/Alc/*.c" }
	files { "src/Alc/backends/null.c", "src/Alc/backends/loopback.c" }
	
	includedirs { "include", "src/OpenAL32/Include" }
	defines { "AL_LIBTYPE_STATIC", "AL_ALEXT_PROTOTYPES", "AL_BUILD_LIBRARY" }
	
	configuration "windows"
		files { "src/Alc/backends/mmdevapi.c", "src/Alc/backends/winmm.c" }
		defines 
		{
			"strcasecmp=_stricmp",
			"strncasecmp=_strnicmp",
			"snprintf=_snprintf",
			"isfinite=_finite",
			"isnan=_isnan"
		}
	
	configuration "linux"
		files { "src/Alc/backends/alsa.c", "src/Alc/backends/pulseaudio.c" }
	
	configuration "macosx or ios"
		files { "src/Alc/backends/coreaudio.c" }
	
	configuration "android"
		files { "src/Alc/backends/opensl.c" }

	configuration "pnacl"
		files { "src/Alc/backends/ppapi.c" }

	configuration "not vs*"
		defines { "HAVE_GCC_DESTRUCTOR" }

	configuration { "not vs*", "windows" }
		excludes { "src/Alc/backends/mmdevapi.c" } -- mingw does not ship with MMdev headers

	configuration ""