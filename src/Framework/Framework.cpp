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
#include <vapor/input/InputManager.h>

namespace vapor {

using namespace vapor::input;

//-----------------------------------//

Framework::Framework(const std::string app, const char** argv)
	:  Engine(app, argv, false), numFrames( 0 ),
	minFrameTime( std::numeric_limits< double >::max() ),
	maxFrameTime( 0.0 ), sumFrameTime( 0.0 ), lastFrameTime( 0.0 )
{
	info( "framework", "Engine framework getting into action" );
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

	// register input callbacks
	registerCallbacks();

	// app-specific initialization
	onInit();

	// set up all the resources
	onSetupResources();

	// set up the scene
	onSetupScene();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::render()
{
	render::Device* renderDevice = getRenderDevice();

	while( frameTimer.reset(), renderDevice->getWindow().pumpEvents() )
	{
		update( lastFrameTime );

		// User update callback.
		onUpdate( lastFrameTime );

		// User rendering callback.
		onRender();

		// Update the active target (swaps buffers).
		renderDevice->updateTarget();

		lastFrameTime = frameTimer.getElapsedTime();

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
	numFrames++;

	minFrameTime = std::min( minFrameTime, lastFrameTime );
	maxFrameTime = std::max( maxFrameTime, lastFrameTime );

	sumFrameTime += lastFrameTime;
	avgFrameTime = sumFrameTime / numFrames;
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