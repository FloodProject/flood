/************************************************************************
*
* vaporEngine by triton © (2008-2009)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)

#else

int main(int argc, char *argv[])

#endif
{
	vapor::Framework* example = new VAPOR_EXAMPLE_NAME();
	example->run();
	delete example;

	#if defined(VAPOR_MEM_LEAK_DETECTOR) && defined(VAPOR_PLATFORM_WINDOWS)
		_CrtDumpMemoryLeaks();
	#endif

	return EXIT_SUCCESS;
}