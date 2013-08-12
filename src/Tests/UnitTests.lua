project "UnitTests"

	uuid "EE6EEE36-5946-4DFF-BEB8-06A99B864148"

	kind "ConsoleApp"
	debugdir "../Core/Test/"
	
	defines { Core.defines }
	
	SetupNativeProjects()

	files
	{
		"UnitTests.lua",
		"**.cpp",
		"**.h",
		"../Core/Test/**",
	}

	vpaths
	{
		["*"] = { ".", path.join(incdir,"UnitTests") },
	}
	
	includedirs
	{
		incdir,
	}
	
	libdirs { Core.libdirs, bindir }
	deps { Core.deps, "UnitTest++" }
	links { Core.name, Core.links }