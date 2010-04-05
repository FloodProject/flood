/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <vapor/Engine.h>
#include <vapor/Timer.h>
#include <vapor/input/Mouse.h>
#include <vapor/input/Keyboard.h>

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

	Framework(const std::string app, const char** argv);
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
	virtual void onUpdate( double delta ) = 0;

	// Renders the scene (called once per frame).
	virtual void onRender() = 0;

	// Input callback functions.
	virtual void onKeyPressed( const input::KeyEvent& );
	virtual void onButtonPressed( const input::MouseButtonEvent& );
	virtual void onButtonReleased( const input::MouseButtonEvent& );

protected:

	Timer frameTimer;
	double lastFrameTime;
	double minFrameTime;
	double maxFrameTime;
	double avgFrameTime;
	double sumFrameTime;
	uint64 numFrames;

private:

	// Register input devices callbacks.
	void registerCallbacks();

	// Calculates some statistics about frame times.
	void updateFrameTimes();

	// Initializes the engine.
	virtual void init();

	// Main rendering loop ( calls onRender() ).
	virtual void render();
};

//-----------------------------------//

} // end namespace		
