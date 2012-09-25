local version = "2.12"
local repo = "git|git://github.com/mono/mono.git"
local license = "LGPL"

project "Mono"
	
	kind "SharedLib"
	
	defines
	{
		"HAVE_CONFIG_H",
	}
	
	-- sgen defines
	defines
	{
		"HAVE_SGEN_GC",
		"HAVE_MOVING_COLLECTOR",
		"HAVE_WRITE_BARRIERS"
	}
	
	includedirs
	{
		"./",
		"mono/",
		"eglib/src/"
	}
	
	files
	{
		"eglib/src/*.c",
		"mono/metadata/*.c",
		"mono/utils/*.c",
	}
	
	excludes
	{	
		"mono/metadata/boehm-gc.c",
		
		"mono/metadata/console-unix.c",
		"mono/metadata/console-win32.c",
		
		--"mono/metadata/coree.c",
		"mono/metadata/monodiet.c",
		"mono/metadata/monosn.c",
		"mono/metadata/null-gc.c",
		"mono/metadata/pedump.c",
		"mono/metadata/tpool-*.c",
		"mono/metadata/sgen-fin-weak-hash.c",

		"mono/utils/mono-embed.c",
	}
	
	configuration "windows"
		files
		{
			"eglib/src/*-win32.c"
		}
		excludes
		{
			"eglib/src/*-unix.c"
		}
		defines
		{
			"WIN32_THREADS",
			"WINVER=0x0500",
			"_WIN32_WINNT=0x0500",
			"_WIN32_IE=0x0501",
			"_UNICODE", "UNICODE",
			"FD_SETSIZE=1024"
		}
		links
		{
			"MonoMini",
			"Mswsock",
			"ws2_32",
			"psapi",
			"version",
			"winmm",
		}
		
	configuration "linux"
		files
		{
			"mono/metadata/tpool-epoll.c"
		}
		
	configuration "macosx or freebsd"
		files
		{
			"mono/metadata/tpool-kqueue.c"
		}

	--configuration "not windows"
	--	files { "eglib/src/*-unix.c" }	
	--	excludes { "eglib/src/*-win32.c" }
	
	configuration "vs*"
		files { "msvc/mono.def" }
		defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE" }
		buildoptions
		{
			"/wd4018", -- signed/unsigned mismatch
			"/wd4244", -- conversion from 'x' to 'y', possible loss of data
			"/wd4133", -- incompatible types - from 'x *' to 'y *'
			"/wd4715", -- not all control paths return a value
			"/wd4047", -- 'x' differs in levels of indirection from 'y'
		}
	
	configuration ""

project "MonoMini"
	
	kind "StaticLib"
	
	defines
	{
		"__default_codegen__",
		"HAVE_CONFIG_H",
	}
	
	includedirs
	{
		"./",
		"mono/",
		"eglib/src/"
	}
	
	files
	{
		"mono/mini/*.c",
	}
	
	excludes
	{
		"mono/mini/fsacheck.c",
		"mono/mini/genmdesc.c",
		"mono/mini/main.c",
		
		"mono/mini/mini-darwin.c",
		
		"mono/mini/mini-alpha.c",
		"mono/mini/exceptions-alpha.c",
		"mono/mini/tramp-alpha.c",
		
		"mono/mini/mini-arm.c",
		"mono/mini/exceptions-arm.c",
		"mono/mini/tramp-arm.c",
		
		"mono/mini/mini-hppa.c",
		"mono/mini/exceptions-hppa.c",
		"mono/mini/tramp-hppa.c",		
		
		"mono/mini/mini-ia64.c",
		"mono/mini/exceptions-ia64.c",
		"mono/mini/tramp-ia64.c",
		
		"mono/mini/mini-mips.c",
		"mono/mini/exceptions-mips.c",
		"mono/mini/tramp-mips.c",
		
		"mono/mini/mini-ppc.c",
		"mono/mini/exceptions-ppc.c",
		"mono/mini/tramp-ppc.c",
		
		"mono/mini/mini-s390*.c",
		"mono/mini/exceptions-s390*.c",
		"mono/mini/tramp-s390*.c",
		
		"mono/mini/mini-sparc.c",
		"mono/mini/exceptions-sparc.c",
		"mono/mini/tramp-sparc.c",		
		
		"mono/mini/mini-llvm.c",
		"mono/mini/mini-posix.c",
	}
	
	configuration "x32"
		files
		{
			"mono/mini/mini-x86.c",
			"mono/mini/exceptions-x86.c",
			"mono/mini/tramp-x86.c",	
		}	
		excludes
		{
			"mono/mini/mini-amd64.c",
			"mono/mini/exceptions-amd64.c",
			"mono/mini/tramp-amd64.c",
		}
		
	configuration "x64"
		files
		{
			"mono/mini/mini-amd64.c",
			"mono/mini/exceptions-amd64.c",
			"mono/mini/tramp-amd64.c",		
		}	
		excludes
		{
			"mono/mini/mini-x86.c",
			"mono/mini/exceptions-x86.c",
			"mono/mini/tramp-x86.c",
		}
			
	configuration "windows"
		files
		{
			"mono/mini/mini-windows.c"
		}
		defines
		{
			"WIN32_THREADS",
			"WINVER=0x0500",
			"_WIN32_WINNT=0x0500",
			"_WIN32_IE=0x0501",
			"_UNICODE", "UNICODE",
			"FD_SETSIZE=1024"
		}
	
	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE" }
		buildoptions
		{
			"/wd4018", -- signed/unsigned mismatch
			"/wd4244", -- conversion from 'x' to 'y', possible loss of data
			"/wd4133", -- incompatible types - from 'x *' to 'y *'
			"/wd4715", -- not all control paths return a value
			"/wd4047", -- 'x' differs in levels of indirection from 'y'
		}
	
	configuration ""