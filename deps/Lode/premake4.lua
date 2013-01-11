project "LodePNG"

	SetupDependencyProject()
		
	local version = "20100314"
	local license = "BSD"
	local archive = "http://lpi.googlecode.com/svn/trunk/lodepng.{h,cpp}"
	
  kind "StaticLib"
  files { "lodepng.cpp" }
 
project "PicoPNG"

	SetupDependencyProject()
		
	local version = "20101224"
	local license = "BSD"
	local archive = "http://lodev.org/lodepng/picopng.cpp"

  kind "StaticLib"
  files { "picopng.cpp" }
 