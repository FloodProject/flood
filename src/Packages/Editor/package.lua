
local packageDependencies = {
    "GUI"
}

group("Packages")

--TODO rename to Editor when premake support groups and projects with same name.
SetupPackage("EditorPackage", packageDependencies)

flags { "Unsafe" }