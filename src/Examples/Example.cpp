#include "Engine.h"

#include <math.h>

using namespace vapor::resources;
using namespace vapor::renderer;

int main(int argc, char *argv[])
{
	// create the logger
	Log logger(ENGINE_NAME, "log.html");
	
	// create display window
	Window &window = Window::createWindow("Example", 320, 240, false);
	
	// load png image
	PNG png("media/triton.png");

	// load ms3d mesh
	Milkshape3D model("cubo.ms3d");

	float angle = 0.0f;

	do 
	{
		glClearColor(sin(angle), sin(angle), sin(angle), 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		angle += 0.1;

		window.update();

	} while ( window.events() );

	return EXIT_SUCCESS;
}