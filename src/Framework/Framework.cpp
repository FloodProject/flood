/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <vapor/PCH.h>

#include <vapor/Framework.h>

#include <limits>

namespace vapor {

//-----------------------------------//

Framework::Framework(const std::string app, const char** argv)
	:  Engine(app, argv, false), numFrames( 0 ),
	// written like this because conflicts with min/max of windows.h
	minFrameTime( (std::numeric_limits< double >::max)() ), 
	maxFrameTime( 0.0 ), sumFrameTime( 0.0 )
{
	info( "framework", "Engine framework getting into action" );
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

		frameTimer.reset();

		// main rendering by app
		onRender();

		// update the active target
		renderDevice->updateTarget();

		lastFrameTime = frameTimer.getDeltaTime();

		updateFrameTimes();
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

} // end namespace

