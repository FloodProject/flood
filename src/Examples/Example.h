/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

#define VAPOR_USE_NAMESPACES
#include <vapor/Headers.h>

#include <iostream>

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
	virtual void onKeyPressed( const input::KeyEvent& );
	virtual void onButtonPressed( const input::MouseButtonEvent& );

private:

	gui::LabelPtr label;
	resources::MS3DPtr mesh;
	resources::SoundPtr snd;
	scene::SoundPtr sound;
	input::InputMap* inputMap; 
	render::TexturePtr tex;
	scene::CameraPtr cam;
	math::Color c;

	bool runLoop;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>