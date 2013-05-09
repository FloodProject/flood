project "cURL"

  SetupNativeDependencyProject()
  DoNotBuildInNativeClient()

  local version = "7.26.0"
  
  kind "StaticLib"
  language "C"
  files { "src/*.c" }
  includedirs { "include", "../zlib/include" }
  defines { "CURL_STATICLIB", "BUILDING_LIBCURL", "HTTP_ONLY" }