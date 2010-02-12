/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

#define VAPOR_USE_NAMESPACES
#include <vapor/Headers.h>

//-----------------------------------//

/**
 * Simple example.
 */

class Example : public vapor::Framework
{
public:

	Example(const char** argv = nullptr);

protected:

	/// Initializes the app.
	virtual void onInit();	

	/// Sets up all the needed resources.
	virtual void onSetupResources();

	/// Called to set up the scene
	virtual void onSetupScene();

	/// Called to update stuff
	virtual void onUpdate( double delta );

	/// Renders the scene
	virtual void onRender();

	/// Callback functions
	virtual void onKeyPressed( const input::KeyEvent& );

private:

	scene::CameraPtr cam;
	gui::LabelPtr label;
	resources::SoundPtr snd;
	scene::SoundPtr sound;
	input::InputMap* inputMap; 
	render::TexturePtr tex;
	math::Color c;

	float fpsUpdateTime;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
