project "dlmalloc"

	SetupNativeDependencyProject()
		
	local version = "2.8.5"
	local license = "Public Domain"
	local archive = "ftp://g.oswego.edu/pub/misc/malloc.c"
	
  	kind "StaticLib"
  	language "C"
  	files { "malloc.c" }
  	
  	defines
  	{
  		"ONLY_MSPACES"
	}