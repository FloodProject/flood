#include <vapor/Engine.h>
#include <vapor/RenderDevice.h>
#include <vapor/OpenGL_Device.h>
#include <vapor/Math.h>

using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::math;

int main(int argc, char *argv[])
{
	// create the logger
	Log logger(ENGINE_NAME, "log.html");

	// create renderer device
	OpenGLDevice device();

	// load png image
	PNG png("media/triton.png");

	// load ms3d mesh
	Milkshape3D model("media/cubo.ms3d");

	return EXIT_SUCCESS;
}