project "Bullet"

	SetupNativeDependencyProject()
	
	local version = "2.80"
	local revision = "2531"
	local repo = "svn:http://bullet.googlecode.com/svn/trunk/"
	local license = "zlib"
	
	kind "StaticLib"

	files 
	{
		"src/BulletCollision/**.h",
		"src/BulletCollision/**.cpp",
		"src/BulletDynamics/**.h",
		"src/BulletDynamics/**.cpp",
		"src/LinearMath/**.h",
		"src/LinearMath/**.cpp",
	}
	
	includedirs { "include" }

--include("src/BulletCollision")
--include("src/BulletDynamics")
--include("src/LinearMath")