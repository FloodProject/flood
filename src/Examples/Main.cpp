/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Example.h"

using namespace vapor::examples;

#ifdef VAPOR_PLATFORM_WINDOWS

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)

#else

int main(int argc, char *argv[])

#endif
{
	Example* example = new Example();
	example->run();
	delete example;

	#ifdef VAPOR_MEM_LEAK_DETECTOR
		_CrtDumpMemoryLeaks();
	#endif

	return EXIT_SUCCESS;
}