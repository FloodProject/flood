/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/Framework.h>

namespace vapor {

//-----------------------------------//

Framework::Framework()
	: Engine(false)
{
	info("framework", "Engine framework getting into action");
}

//-----------------------------------//

Framework::~Framework()
{

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
	// create the resource manager
	setupResourceManager();

	// setup the global logger
	setupLogger("vaporEngine Example", "vaporEngine.html");

	// app-specific initialization
	onInit();

	// register default codecs
	setupResourceLoaders();

	// create a rendering and audio device
	setupDevices();

	setupWindow("vaporEngine Example");

	// set up all the resources
	onSetupResources();

	// set up the scene
	onSetupScene();
}

//-----------------------------------//

void Framework::render()
{
	render::Device* renderDevice = getRenderDevice();

	while(renderDevice->getWindow()->pumpEvents())
	{
		// update time!
		onUpdate();

		// main rendering by app
		onRender();

		// update the active target
		renderDevice->updateTarget();
	}	
}

//-----------------------------------//

} // end namespace

