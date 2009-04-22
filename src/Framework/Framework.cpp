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
	namespace framework {

Framework::Framework()
	: device(nullptr), window(nullptr), resourceManager(nullptr)
{
	info("framework", "Starting a vaporEngine Framework-based application");
}

Framework::~Framework()
{
	cleanup();
}

void Framework::run()
{
	init();
	render();
}

void Framework::init()
{
	// create render device
	device = &Device::createDevice();

	// set the window title
	device->getWindow().setTitle("vaporEngine Example");

	// create the resource manager
	resourceManager = new ResourceManager();

	// more specific initialization
	onInit();

	// set up all the resources
	onSetupResources();
}

void Framework::render()
{
	do {
		// main rendering by app
		onRender();

		// update the active target
		device->updateTarget();

	} while( device->getWindow().pump() );
}

void Framework::cleanup()
{
	// free all objects
	delete device;
	delete resourceManager;
	delete log::Log::getLogger();
}

} } // end namespaces