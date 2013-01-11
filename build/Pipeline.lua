Pipeline = {}
Pipeline.name = "Pipeline"
Pipeline.shared = true
Pipeline.defines = {}

function SetupFBX()
	-- "../dep/FbxSdk/2012.1/lib/vs2010/x86",
end

project "Pipeline"

	if Pipeline.shared then
		kind "SharedLib"
		table.insert(Pipeline.defines, "API_PIPELINE_DLL")
		defines { "API_PIPELINE_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	builddeps { "Core", "Resources", "Graphics", "Engine" }
	
	pchheader "Pipeline/API.h"
	pchsource "../src/Pipeline/Pipeline.cpp"
	
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
		"../inc/Pipeline/**.h",
		"../src/Pipeline/**.cpp",
	}
	
	vpaths
	{
		[""] = { "../src/", "../inc/" },
		["Importers"] = { "**/*Importer*" },
		["Importers/Milkshape"] = { "**/*Milkshape*" },
		["Importers/FBX"] = { "**/*FBX*" },
		["Processors"] = "**/*Processor*",
		["Processors/Image"] = "**/*ImageProcessor*",
		["Processors/Mesh"] = "**/*MeshProcessor*",
	}

	includedirs
	{
		"../src/",
		"../inc/",
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