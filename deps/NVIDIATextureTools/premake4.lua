project "NVIDIATextureTools"
	
	local version = "2.1.0"
	local repo = "svn|http://nvidia-texture-tools.googlecode.com/svn/trunk/"
	local license = "MIT"
	
	kind "StaticLib"
	files { "src/**.cpp" }
	
	excludes {
		"src/nvimage/KtxFile.cpp",
		"src/nvtt/CompressorDX11.cpp",
		"src/nvtt/experimental/**.cpp",
		-- ignore the bc7 compressor
		"src/nvtt/bc7/**.cpp",
		-- ignore extra squish files
		"src/nvtt/squish/extra/*.cpp",
		"src/nvtt/squish/alpha.cpp",
		"src/nvtt/squish/clusterfit.cpp",
		"src/nvtt/squish/fastclusterfit.cpp",
		"src/nvtt/squish/rangefit.cpp",
		"src/nvtt/squish/singlechannelfit.cpp",
		"src/nvtt/squish/singlecolourfit.cpp",
		"src/nvtt/squish/squish.cpp",
	}
	
	includedirs { "src/", "extern/poshlib/" }
	defines { "__MMX__", "__SSE2__", "__SSE__" }