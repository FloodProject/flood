#include "Core.h"

Core::Core(const string& logFile)
{
	createWindow();
	logger = new Log("", logFile);
}

Core::~Core()
{
	delete window;
	delete logger;
}

void Core::createWindow(const string& title, const int width,
		const int height, const bool fullscreen)
{
	//window = new Window(title, width, height, fullscreen);
}