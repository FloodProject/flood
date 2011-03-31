/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Render/FrameStatistics.h"
#include "Core/Timer.h"
#include "Engine.h"

namespace vapor {

//-----------------------------------//

 /**
 * Simple framework to facilitate the use of the engine functionality in
 * applications. It asks for a rendering device to use, and has some 
 * virtual methods used to setup all the needed resources. Applications 
 * can just derive from this class and setup the needed stuff, call the 
 * run() method and everything should automagically work.
 */

class VAPOR_API Framework : public Engine
{
public:

	Framework(const std::string& app, const char** argv);
	virtual ~Framework() { }

	// Kickstart the framework in action.
	void run();

protected:

	// Called once when the app starts.
	virtual void onInit() = 0;

	// Sets up all the needed resources.
	virtual void onSetupResources() = 0;

	// Sets up all the scene nodes.
	virtual void onSetupScene() = 0;

	// Updates the scene (called once per frame).
	virtual void onUpdate( float delta ) = 0;

	// Renders the scene (called once per frame).
	virtual void onRender() = 0;

	// Input callback functions.
	virtual void onKeyPressed( const KeyEvent& ) {}
	virtual void onButtonPressed( const MouseButtonEvent& ) {}
	virtual void onButtonReleased( const MouseButtonEvent& ) {}

protected:

	// Register input devices callbacks.
	void registerCallbacks();

	// Calculates some statistics about frame times.
	void updateFrameTimes();

	// Initializes the engine.
	virtual void init();

	// Main rendering loop.
	virtual void mainLoop();

	// Frame loop timer.
	Timer frameTimer;

	// Holds frame statistics.
	FrameStatistics frameStats;
};

//-----------------------------------//

} // end namespace		
