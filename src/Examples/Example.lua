project "Example"

	kind "WindowedApp"
	location (action)
	objdir (builddir .. "/obj")
	targetdir (bindir)
	
	SetupNativeProjects()
	
	files
	{
		"**.h",
		"**.cpp",
		"**.cpp",
	}

	includedirs {
		srcdir,
		incdir,
		"../inc/vapor",
		"../dep/freetype/include",
		"../dep/glew/include",
		"../dep/lua/include",
		"../dep/openal/include",
		"../dep/bullet/include"
	}
	
	libdirs {
		libdir,
		"../dep/physfs/",
		"../dep/sfml/"
	}
	
	links {
		"Core", "Engine", "Resources",
		"physfs", "sfml-window.framework",
		"OpenAL.framework", "OpenGL.framework"
	}