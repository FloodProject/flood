Core = {}
Core.name = "Core"
Core.isShared = true
Core.defines = {}

project "Core"

    uuid "2B0C5D93-3873-4AB3-81FD-2D41965DD031"

    if Core.isShared then
        kind "SharedLib"
        table.insert(Core.defines, "API_CORE_DLL")
        defines { Core.defines, "API_CORE_DLL_EXPORT" }
    else
        kind "StaticLib"
    end
    
    SetupNativeProjects()

    pchheader "Core/API.h"
    pchsource "Core.cpp"

    files
    {
        path.join(builddir,"../Config.lua"),
        "Core.lua",
        path.join(incdir,"Core","**.h"),
        "**.cpp",
    }

    vpaths
    {
        ["*"] = { ".", path.join(incdir,"Core") },
        ["Platforms/*"] = { path.join( srcdir,"Platforms") },
    }

    excludes
    {
        "Test/**",
    }
    
    includedirs
    {
        incdir,
        path.join(depsdir,"Dirent")
    }
    
    Core.links = {}
    
    Core.libdirs = {}

    Core.deps =
    {
        "dlmalloc",
        "FastLZ",
        "Jansson",
        "zziplib",
        "zlib",
        "tropicssl"
    }

    Core.extradeps =
    {
        "cURL",
        "ENet",
    }

    configuration "windows"
    
        files { path.join(srcdir, "Platforms/Win32/FileWatcherWin32.cpp") }
        files { path.join(srcdir, "Platforms/Win32/ConcurrencyWin32.cpp") }
        
        links { "ws2_32", "winmm" }
        deps { Core.extradeps }
        --table.insert(Core.links, "ws2_32")
        --table.insert(Core.links, "winmm")

    configuration "pnacl"
        files
        {
            path.join(srcdir, "Platforms/Posix/ThreadPosix.cpp"),
            path.join(srcdir, "Platforms/NaCl/NaclModule.cpp"),
        }
        links { "c" }

    configuration "vs* and not pnacl"

        -- Setup Visual Leak Detector
        if config.MEMORY_LEAK_DETECTOR then
            table.insert(Core.links, "vld")
            table.insert(Core.libdirs,
                path.join(depsdir, "VisualLeakDetector/lib/Win32"))
        end

    configuration {}

    links { Core.links }
    libdirs { Core.libdirs }
    deps(Core.deps)
