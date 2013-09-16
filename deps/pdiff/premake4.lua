project "pdiff"

    SetupNativeDependencyProject()

    kind "ConsoleApp"
    language "C++"
    
    files { "*.h", "*.cpp"}
    includedirs 
    { 
        ".",
        path.join(depsdir,"FreeImage","Source")
    }

    links { "FreeImage" }
