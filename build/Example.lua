project "Example"

	kind "WindowedApp"
	location (action)
	objdir (action .. "/obj")
	targetdir ("../bin")
	
	flags { common_flags }
	
	files {
		"../src/Examples/**.h",
		"../src/Examples/**.cpp",
		"../src/Framework/**.cpp",
	}

	includedirs {
		"../src",
		"../inc/",
		"../inc/vapor",
		"../dep/freetype/include",
		"../dep/glew/include",
		"../dep/lua/include",
		"../dep/openal/include",
		"../dep/bullet/include"
	}
	
	libdirs {
		path.join(action, "lib"),
		"../dep/physfs/",
		"../dep/sfml/"
	}
	
	links {
		"Core", "Engine", "Resources",
		"physfs", "sfml-window.framework",
		"OpenAL.framework", "OpenGL.framework"
	}