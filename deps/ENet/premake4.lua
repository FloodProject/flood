project "ENet"

  SetupDependencyProject()
  
  local version = "1.3.4"
  local repo = "https://github.com/lsalzman/enet.git"
  local license = "BSD-like"
  
  kind "StaticLib"
  files { "src/*.c" }
  includedirs { "include" }
  defines { "ENET_BUILDING_LIB" }