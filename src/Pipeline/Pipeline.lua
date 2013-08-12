Pipeline = {}
Pipeline.name = "Pipeline"
Pipeline.shared = true
Pipeline.defines = {}

function SetupFBX()
	-- "../dep/FbxSdk/2012.1/lib/vs2010/x86",
end

project "Pipeline"

	uuid "65CCD652-683D-4626-A525-F30307C0429F"

	if Pipeline.shared then
		kind "SharedLib"
		table.insert(Pipeline.defines, "API_PIPELINE_DLL")
		defines { "API_PIPELINE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	pchheader "Pipeline/API.h"
	pchsource "Pipeline.cpp"
	
	defines
	{
	 	Core.defines,
	 	Resources.defines,
	 	Graphics.defines,
	 	Engine.defines,
	 	Pipeline.defines
	}

	files
	{
		"Pipeline.lua",
		path.join(incdir,"Pipeline/**.h"),
		"**.cpp",
	}
	
	vpaths
	{
		[""] = { ".", incdir },
		["Importers"] = { "**/*Importer*" },
		["Importers/Milkshape"] = { "**/*Milkshape*" },
		["Importers/FBX"] = { "**/*FBX*" },
		["Processors"] = "**/*Processor*",
		["Processors/Image"] = "**/*ImageProcessor*",
		["Processors/Mesh"] = "**/*MeshProcessor*",
	}

	includedirs
	{
		srcdir,
		incdir,
	}

	Pipeline.libdirs =
	{
	
	}

	libdirs { Pipeline.libdirs }

	SetupFBX()
	
	Pipeline.links =
	{
		Core.name,
		Resources.name,
		Graphics.name,
		Engine.name
	}

	links { Pipeline.links }

	Pipeline.deps =
	{
		"NVIDIATextureTools"
	}
	
	deps(Pipeline.deps)