-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

dofile "Helpers.lua"

ImportConfigs()

print("Generating the build configuration 'Build.h'")
local conf = GenerateBuildConfig(config)
WriteConfigToFile(conf, "../Build.h")

solution "Flood"
	
	configurations { "Debug", "Release" }
	platforms { "x32", "x64", "pnacl" }
	defaultplatform "x32"
	
	flags { common_flags }
	language "C++"
	framework "4.5"
	
	location (builddir)
	objdir (builddir .. "/obj/")
	targetdir (libdir)
	libdirs { libdir }
	
	debugdir (bindir)
	startproject "EditorRuntime"

	-- Build configuration options

	configuration "pnacl"
		system "nacl"
		architecture "pnacl"
	
	configuration "Release"
		flags { "Optimize" }

	configuration "Debug"
		defines { "DEBUG" }
		
	configuration {}
	
	group "Native"
	
		dofile( srcdir .. "/Core/Core.lua")
		dofile( srcdir .. "/Resources/Resources.lua")
		dofile( srcdir .. "/Graphics/Graphics.lua")
		dofile( srcdir .. "/Engine/Engine.lua")
		dofile( srcdir .. "/Pipeline/Pipeline.lua")

	--group "Runtimes"

		--dofile( srcdir .. "/Runtime/Runtime.lua")
		--dofile( srcdir .. "/GameRuntime/GameRuntime.lua")

	group "Managed"

		dofile( srcdir .. "/EngineManaged/Bindings/EngineBindings.lua")
		dofile( srcdir .. "/EngineManaged/Bindings.CSharp/EngineBindings.CSharp.lua")
        dofile( srcdir .. "/EngineBindings.Tests/EngineBindings.Tests.lua")
		dofile( srcdir .. "/EngineManaged/EngineManaged.lua")
		dofile( srcdir .. "/EngineManaged.Tests/EngineManaged.Tests.lua")
		dofile( srcdir .. "/ServerManaged/ServerManaged.lua")
        
	group "Editor"
	
		dofile( srcdir .. "/Editor/Editor.Client/Editor.Client.lua")
		dofile( srcdir .. "/EditorRuntime/EditorRuntime.lua")
		dofile( srcdir .. "/Editor/Editor.Shared/Editor.Shared.lua")
		dofile( srcdir .. "/Editor/Editor.Server/Editor.Server.lua")
		dofile( srcdir .. "/Editor/Editor.Tests/Editor.Tests.lua")

	group "Editor/Addins"

    	print("Searching for addins...")
    	IncludePremakeProjects(path.join(srcdir, "Editor","Addins", "*"))		
        
	group "Tests"

		dofile( srcdir .. "/Tests/UnitTests.lua")
		
	group "Tools"

		if config.BINDINGGEN then
			dofile( srcdir .. "/Tools/BindingsGen/BindingsGen.lua")
		end

		if config.BAKEFILEGEN then
			dofile( srcdir .. "/Tools/BakefileGen/BakefileGen.lua")
		end

		dofile( srcdir .. "/Tools/PackageGen/PackageGen.lua")
		dofile( srcdir .. "/Tools/RPCGen/RPCGen.lua")
        dofile( srcdir .. "/Tools/RPCGen.Tests/RPCGen.Tests.lua")
		dofile( srcdir .. "/Tools/EngineWeaver/EngineWeaver.lua")
		dofile( srcdir .. "/Tools/EngineWeaver.Tests/EngineWeaver.Tests.lua")

	group "Examples"

        print("Searching for externals projects...")
        IncludePremakeProjects(path.join(examplesdir, "*"))	

    group "Externals"

        print("Searching for externals projects...")
        IncludePremakeProjects(path.join(extdir, "*"))
