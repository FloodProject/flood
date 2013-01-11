project "AnyOption"

  SetupDependencyProject()
  
  kind "StaticLib"
  files { "src/*.cpp" }
  includedirs { "include" }