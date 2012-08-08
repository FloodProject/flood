project "UnitTests"

	kind "ConsoleApp"
	builddeps { "Core" }
	debugdir "../src/Core/Test/"
	
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
	
	links { Core.links, "Core" }
	libdirs { Core.libdirs }
	deps { Core.deps, "UnitTest++" }