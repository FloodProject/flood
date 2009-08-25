/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

#ifndef VAPOR_EXAMPLE_NAME
	#error "vapor::Framework code needs you need to define VAPOR_EXAMPLE_NAME.
#endif

#ifdef VAPOR_PLATFORM_WINDOWS
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	VAPOR_EXAMPLE_NAME example;
	example.run();

	#if defined(VAPOR_MEMORY_LEAK_DETECTOR) && defined(VAPOR_PLATFORM_WINDOWS)
		//_CrtDumpMemoryLeaks();
	#endif

	return EXIT_SUCCESS;
}