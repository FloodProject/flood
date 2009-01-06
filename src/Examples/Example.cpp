#include "Framework.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
	EngineFramework *example = new EngineFramework();

	example->run();

	delete example;

	return EXIT_SUCCESS;
}
