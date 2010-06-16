/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <vapor/PCH.h>
#include <vapor/Framework.h>
#include <vapor/render/Device.h>
#include <vapor/vfs/FileSystem.h>
#include <vapor/input/InputManager.h>
#include <vapor/resources/ResourceManager.h>

namespace vapor {

//-----------------------------------//

Framework::Framework(const std::string& app, const char** argv)
{
	info( "framework", "Engine framework getting into action" );
	Engine::create(app, argv, false);
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
	// Init the engine.
	Engine::init( true );

	// Register input callbacks.
	registerCallbacks();

	// Register default locations.
	FileSystem* fs = getFileSystem();
	fs->mountDefaultLocations();

	// User init callback.
	onInit();

	// User resources setup callback.
	onSetupResources();

	// User scene setup callback.
	onSetupScene();

	// Wait until all resources are loaded.
	resourceManager->waitUntilQueuedResourcesLoad();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::render()
{
	RenderDevicePtr renderDevice = getRenderDevice();
	WindowPtr window = renderDevice->getWindow();

	while( frameTimer.reset(), window->pumpEvents() )
	{
		update( frameStats.lastFrameTime );

		// User update callback.
		onUpdate( frameStats.lastFrameTime );

		// User rendering callback.
		onRender();

		// Update the active target (swaps buffers).
		window->update();

		updateFrameTimes();
	}

	Timer::sleep( 0 );
}

//-----------------------------------//

void Framework::registerCallbacks()
{
	Keyboard* kbd = getInputManager()->getKeyboard();
	Mouse* mouse = getInputManager()->getMouse();

	if( kbd )
	{
		kbd->onKeyPress += fd::bind( &Framework::onKeyPressed, this );
	}
	
	if( mouse )
	{
		mouse->onMouseButtonPress += fd::bind( &Framework::onButtonPressed, this );
		mouse->onMouseButtonRelease += fd::bind( &Framework::onButtonReleased, this );
	}
}

//-----------------------------------//

void Framework::updateFrameTimes()
{
	frameStats.lastFrameTime = frameTimer.getElapsedTime();

	frameStats.numFrames++;

	frameStats.minFrameTime = std::min( frameStats.minFrameTime, frameStats.lastFrameTime );
	frameStats.maxFrameTime = std::max( frameStats.maxFrameTime, frameStats.lastFrameTime );

	frameStats.sumFrameTime += frameStats.lastFrameTime;
	frameStats.avgFrameTime = frameStats.sumFrameTime / frameStats.numFrames;
}

//-----------------------------------//

void Framework::onKeyPressed( const KeyEvent& )
{

}

//-----------------------------------//

void Framework::onButtonPressed( const MouseButtonEvent& )
{

}

//-----------------------------------//

void Framework::onButtonReleased( const MouseButtonEvent& )
{

}

//-----------------------------------//

} // end namespace