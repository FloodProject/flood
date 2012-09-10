project "SDL"

	local version = "2.0"
	local repo = "git://repo.or.cz/openal-soft.git"
	local archive = "http://kcat.strangesoft.net/openal-releases/openal-soft-1.14.tar.bz2"
	local license = "BSD"
	
	kind "StaticLib"
	
	files
	{
		"premake4.lua",

	-- Source files
		"src/*.c",
		"src/atomic/*.c",
		"src/audio/dummy/*.c",
		"src/cpuinfo/*.c",
		"src/events/*.c",
		"src/file/*.c",
		"src/haptic/*.c",
		"src/haptic/dummy/*.c",
		"src/joystick/*.c",
		"src/joystick/dummy/*.c",
		"src/loadso/*.c",
		"src/loadso/dummy/*.c",
		"src/power/*.c",
		"src/power/dummy/*.c",
		"src/render/*.c",
		"src/render/opengl/*.c",
		"src/stdlib/*.c",
		"src/thread/*.c",
		"src/timer/*.c",
		"src/timer/dummy/*.c",
		"src/video/*.c",
		"src/video/dummy/*.c",

	-- Header files
		"include/*.h",
		"src/*.h",
		"src/atomic/*.h",
		"src/audio/dummy/*.h",
		"src/cpuinfo/*.h",
		"src/events/*.h",
		"src/file/*.h",
		"src/haptic/*.h",
		"src/haptic/dummy/*.h",
		"src/joystick/*.h",
		"src/joystick/dummy/*.h",
		"src/loadso/*.h",
		"src/loadso/dummy/*.h",
		"src/power/*.h",
		"src/power/dummy/*.h",
		"src/render/*.h",
		"src/render/opengl/*.h",
		"src/stdlib/*.h",
		"src/thread/*.h",
		"src/timer/*.h",
		"src/timer/dummy/*.h",
		"src/video/*.h",
		"src/video/dummy/*.h",
	}
	
	includedirs { "include" }
	
	configuration "windows"
		files
		{ 
		-- Source files
			"src/core/windows/*.c",
			"src/haptic/windows/*.c",
			"src/joystick/windows/*.c",
			"src/loadso/windows/*.c",
			"src/main/windows/*.c",
			"src/power/windows/*.c",
			"src/thread/windows/*.c",
			"src/timer/windows/*.c",
			"src/video/windows/*.c",

		-- Header files
			"src/core/windows/*.h",
			"src/haptic/windows/*.h",
			"src/joystick/windows/*.h",
			"src/loadso/windows/*.h",
			"src/main/windows/*.h",
			"src/power/windows/*.h",
			"src/thread/windows/*.h",
			"src/timer/windows/*.h",
			"src/video/windows/*.h",
		}