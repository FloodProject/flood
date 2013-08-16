-- This module checks for the all the project dependencies.

action = _ACTION or ""

depsdir = path.getabsolute("../../deps");
extdir = path.getabsolute("../../externals");
srcdir = path.getabsolute("../../src");
incdir = path.getabsolute("../../inc");
bindir = path.getabsolute("../../bin");
examplesdir = path.getabsolute("../../examples");
builddir = path.getabsolute("../" .. action);
libdir = path.join(builddir, "lib","%{cfg.buildcfg}_%{cfg.platform}");
addindir = path.join(libdir, "Addins");
gendir = path.join(builddir, "gen");

common_flags = { "Unicode", "Symbols", "NoExceptions", "NoRTTI" }
msvc_buildflags = { "/wd4190", "/wd4996", "/wd4530" }
gcc_buildflags = { "-Wno-invalid-offsetof", "-std=gnu++11" }

msvc_cpp_defines = { "_SECURE_SCL=0", "_HAS_ITERATOR_DEBUGGING=0" }

function debug(msg)
	-- print(msg)
end

function SafePath(path)
	return "\"" .. path .. "\""
end

-- Copy a configuration build header if one does not exist yet.
function ImportConfigs()
    if not os.isfile("../Config.lua") then
        print("Build configuration file 'Config.lua' created")
        os.copyfile("Config0.lua", "../Config.lua")
    end
    dofile "../Config.lua"
end

function IncludePremakeProjects(basedir)
    local deps = os.matchdirs(basedir)
    
    for i,dep in ipairs(deps) do
        local fp = path.join(dep, "premake4.lua")
        fp = path.join(os.getcwd(), fp)
        
        if os.isfile(fp) then
            print(string.format(" including %s", dep))
            include(dep)
        end
    end
end

function SetupRPCGen(projectName,extension)
	dependson { "RPCGen" }

	local exePath = SafePath("$(TargetDir)" .. "RPCGen.exe")
	local outPath = SafePath(path.join(gendir,projectName))
	local dllPath = SafePath("$(TargetDir)" .. projectName .. extension)
	local command = exePath .. " -o=" .. outPath .. " " .. dllPath;
    
	postbuildcommands { command }
end

function SetupEngineWeaver(dllName)
	dependson { "EngineWeaver" }

	local exePath = SafePath("$(TargetDir)" .. "EngineWeaver.exe");
	local dllPath = SafePath("$(TargetDir)" .. dllName)
	
	postbuildcommands
	{
		exePath .. dllPath
	}
end

function SetupLibPaths(lib)
	local src = path.join(depsdir, lib, "src")
	local include = path.join(depsdir, lib, "include")
	
	if os.isdir(path.getabsolute(include)) then
		debug("Including lib", lib, "from", include)
		includedirs { include }
	elseif os.isdir(path.getabsolute(src)) then
		debug("Including lib", lib, "from", src)
		includedirs { src }
	else
		debug("Including lib", lib, "from", depsdir .. lib)
		includedirs { path.join(depsdir , lib) }
	end

	local libpath = path.join(depsdir, lib, "lib" , action)
	if os.isdir(path.getabsolute(libpath)) then
		libdirs { libpath }
	end
	
	libdirs { libdir }
end

function SetupNativeBuildFlags()
	-- Compiler-specific options

	configuration "pnacl"
		system "nacl"
		architecture "pnacl"
	
	configuration { "vs*", "not pnacl" }
		buildoptions { msvc_buildflags, "/wd4251" }
		defines { msvc_cpp_defines }
		
	configuration "gcc"
		buildoptions { gcc_buildflags }

end

function SetupNativeProjects()
	location (path.join(builddir, "projects"))

	local c = configuration "Debug"
		defines { "_DEBUG" }
		
	configuration "Release"
		defines { "NDEBUG" }
	
	SetupNativeBuildFlags()
	configuration(c)
end

function SetupNativeDependencyProject()
	location (path.join(builddir, "deps"))

	-- Build configuration options
	
	local c = configuration "Debug"
		defines { "_DEBUG" }
		flags { "NoMinimalRebuild", "FloatFast", "NoEditAndContinue" }
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize", "NoMinimalRebuild", "FloatFast" }

	SetupNativeBuildFlags()
	configuration(c)
end

function SetupManagedDependencyProject()
	location (path.join(builddir, "deps"))
end

addins = {}
addinsDepedenciesUsed = false

function SetupAddin(addinName)
    if addinsDepedenciesUsed then
		print("WARNING: Dependencies have been already used!")
    end
    
    table.insert(addins, addinName)
    
    targetdir (addindir)
    SetupAddinResources()
end

function SetupAddinsAsDependencies()
    dependson (addins)
    addinsDepedenciesUsed = true
end

function SetupAddinResources()
    local resources =
    {
        "**.addin.xml", 
        path.join("icons","**.png")
    }
    
    EmbedFiles(resources)
end

function EmbedFiles(resources)
    files (resources)

    local c = configuration (resources[1])
            buildaction "Embed"
            
    for resourceIt = 2, #resources do
        configuration (resources[resourceIt])
            buildaction "Embed"
    end
    
    configuration(c)
end

function DoNotBuildInNativeClient()
	local c = configuration "pnacl"
		buildaction "None"
	configuration(c)
end

function deps(dep)
	for i,lib in ipairs(dep) do
		if type(lib) == "table" then
			deps(lib)
		else
			links {lib}
			SetupLibPaths(lib)
		end
	end
end

function GenerateBuildConfig(conf)
	local contents = ""
	
	for key,value in pairs(conf) do
		local define = string.format("#define ENABLE_%s", key)
		if value then
			--print(string.format("Enabling %s", key))
		else
			define = "//" .. define
		end
		contents = contents .. define .. "\n"
	end
	
	return contents
end

function WriteConfigToFile(conf, path)
	file = io.open(path, "w")
	file:write(conf)
	file:close()
end