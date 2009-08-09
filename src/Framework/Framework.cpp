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

Framework::Framework(const std::string app)
	:  Engine(app, false)
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
	Framework::init();

	// call the render cycle
	render();
}

//-----------------------------------//

void Framework::init()
{
	// init the engine
	Engine::init();

	// app-specific initialization
	onInit();

	// set up all the resources
	onSetupResources();

	// set up the scene
	onSetupScene();
}

//-----------------------------------//

void Framework::render()
{
	render::Device* renderDevice = getRenderDevice();

	while( renderDevice->getWindow()->pumpEvents() )
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

