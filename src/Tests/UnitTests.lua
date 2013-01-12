project "UnitTests"

	kind "ConsoleApp"
	builddeps { "Core" }
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