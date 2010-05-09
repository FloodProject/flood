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
	virtual void onButtonPressed( const input::MouseButtonEvent& );

private:

	scene::CameraPtr cam;

	render::WindowPtr window;
	render::ViewportPtr viewport;
	render::ViewportPtr viewport2;

	gui::LabelPtr label;
	resources::SoundPtr snd;
	render::RenderBufferPtr fbo;
	scene::NodePtr fbo_node;
	//scene::SoundPtr sound;
	input::InputMap* inputMap; 
	render::TexturePtr tex;
	render::TexturePtr fbo_tex;

	float fpsUpdateTime;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
