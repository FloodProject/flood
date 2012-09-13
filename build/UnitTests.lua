project "UnitTests"

	kind "ConsoleApp"
	builddeps { "Core" }
	debugdir "../src/Core/Test/"
	
	defines { Core.defines }

	files
	{
		"UnitTests.lua",
		"../src/Tests/*.cpp",
		"../src/Tests/*.h",
		"../src/Core/Test/**",
	}
	
	includedirs
	{
		"../inc/",
	}
	
	libdirs { Core.libdirs, "../bin/" }
	deps { Core.deps, "UnitTest++" }
	links { Core.links }