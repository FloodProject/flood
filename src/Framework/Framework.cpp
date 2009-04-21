/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/Framework.h>

//#include "Dialog.h"

using namespace vapor::resources;
using namespace vapor::render;

namespace vapor {
	namespace framework {

Framework::Framework()
	: device(nullptr), window(nullptr), resourceManager(nullptr)
{
	init();
	render();
}

Framework::~Framework()
{
	cleanup();
}

void Framework::init()
{
	// create render device
	device = &Device::createDevice();

	// set the window title
	device->getWindow().setTitle("vaporEngine Example");

	// create the resource manager
	resourceManager = new ResourceManager();

	// set up all the resources
	setupResources();
}

//void Framework::ask()
//{
//	Dialog *dialog = new Dialog();
//	delete dialog;
//}

void Framework::setupResources()
{

}

void Framework::render()
{
	do {
		// clear the render device
		device->clearTarget();

		// update the active target
		device->updateTarget();

	} while( device->getWindow().pump() );
}

void Framework::cleanup()
{
	// free all objects
	delete device;
	delete resourceManager;
	delete Log::getLogger();
}

} } // end namespaces