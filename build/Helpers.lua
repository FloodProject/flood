-- This module checks for the all the project dependencies.

function FindFBX()
end

function FindWxWidgets()
	wxWidgets = {}
	wxWidgets.links = {}
end

function FindMono()
	Mono = {}
	Mono.links = { "eglib", "libgc", "mono-2.0" }
end

function SetupLibPaths(lib) 
	c = configuration {}
	
	local include = "../deps/" .. lib .. "/include/"
	if os.isdir(path.getabsolute(include)) then
		--print("Including lib", lib, "from", include)
		includedirs { include }
	else
		includedirs { "../deps/" .. lib }
	end

	local libpath = "../deps/" .. lib .. "/lib/" .. action
	if os.isdir(path.getabsolute(libpath)) then
		libdirs { libpath }
	end
	
	libdirs { "../deps/build/" .. action .. "/lib" }
	
	configuration(c)
end

function SetupLibLinks(lib)
	c = configuration "Debug"
		links { lib .. "_d" }
	configuration "Release"
		links { lib }
	configuration(c)
end

function builddeps(deps)
end

function deps(dep)
	for i,lib in ipairs(dep) do
		if type(lib) == "table" then
			deps(lib)
		else
			SetupLibPaths(lib)
			SetupLibLinks(lib)
		end
	end
end

function startup(prj)
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