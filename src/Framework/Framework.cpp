/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/Framework.h>

using namespace vapor::resources;
using namespace vapor::render;

namespace vapor {

//-----------------------------------//

Framework::Framework()
{
	info("framework", "Engine framework getting into action");
}

//-----------------------------------//

Framework::~Framework()
{
	cleanup();
}

//-----------------------------------//

void Framework::run()
{
	// init the engine
	init();

	// call the render cycle
	render();
}

//-----------------------------------//

void Framework::init()
{
	// setup the global logger
	setupLogger("vaporEngine Example", "vaporEngine.html");

	// app-specific initialization
	onInit();

	// register default codecs
	setupResourceLoaders();

	// create a rendering device
	setupDevice();

	// set up all the resources
	onSetupResources();
}

//-----------------------------------//

void Framework::render()
{
	Device* device = getDevice();

	while(device->getWindow()->pumpEvents())
	{
		// update time!
		onUpdate();

		// main rendering by app
		onRender();

		// update the active target
		device->updateTarget();
	}	
}

//-----------------------------------//

void Framework::cleanup()
{

}

//-----------------------------------//

} // end namespace

