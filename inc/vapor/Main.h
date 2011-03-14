/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

#include <vapor/Framework.h>

#if defined(VAPOR_PLATFORM_WINDOWS) && defined(USE_COMMON_CONTROLS)
	#define SIDEBYSIDE_COMMONCONTROLS 1 
	#include <commctrl.h>
	#pragma comment( lib, "comctl32.lib" )
#endif

int main(int argc, const char** argv)
{
	#if defined(VAPOR_PLATFORM_WINDOWS) && defined(USE_COMMON_CONTROLS)
		InitCommonControls();
	#endif

	#ifdef VAPOR_PLATFORM_WINDOWS
		VAPOR_EXAMPLE_NAME example;
	#else
		VAPOR_EXAMPLE_NAME example( argv );		
	#endif

	example.run();

	return EXIT_SUCCESS;
}