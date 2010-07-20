/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

#if defined(VAPOR_PLATFORM_WINDOWS) && defined(USE_COMMON_CONTROLS)
	#define SIDEBYSIDE_COMMONCONTROLS 1 
	#include <commctrl.h>
	#pragma comment( lib, "comctl32.lib" )
#endif

#ifndef VAPOR_EXAMPLE_NAME
	#error "vapor::Framework code needs you need to define VAPOR_EXAMPLE_NAME."
#endif

#ifdef VAPOR_PLATFORM_WINDOWS
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
#else
int main(int argc, const char** argv)
#endif
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