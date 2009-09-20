/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

#include <vapor/render/VertexBuffer.h>
#include <vapor/render/Renderable.h>
#include <vapor/scene/Camera.h>
#include <vapor/input/InputMap.h>

//-----------------------------------//

/**
 * Simple example.
 */

class Example : public vapor::Framework
{
public:

	Example(const char** argv = nullptr);

protected:

	// Initializes the app.
	virtual void onInit();	

	// Sets up all the needed resources.
	virtual void onSetupResources();

	// Called to set up the scene
	virtual void onSetupScene();

	// Called to update stuff
	virtual void onUpdate( double delta );

	// Renders the scene
	virtual void onRender();

	// Callback functions
	virtual void onKeyPressed( const vapor::input::KeyEvent& );
	virtual void onButtonPressed( const vapor::input::MouseButtonEvent& );
	void leap();
	void Run();

private:

	vapor::input::InputMap * inputMap; 
	vapor::render::TexturePtr tex;
	vapor::scene::CameraPtr cam;
	vapor::math::Color c;
	bool runLoop;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>