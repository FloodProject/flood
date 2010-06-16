/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>
#include <vapor/Headers.h>

using namespace vapor;

//-----------------------------------//

class Example : public vapor::Framework
{
public:

	Example(const char** argv = nullptr);

protected:

	/// Initializes the app.
	virtual void onInit();	

	/// Sets up all the needed resources.
	virtual void onSetupResources();

	/// Called to set up the scene.
	virtual void onSetupScene();

	/// Called to update stuff.
	virtual void onUpdate( double delta );

	/// Renders the scene.
	virtual void onRender();

	/// Callback functions.
	virtual void onKeyPressed( const KeyEvent& );
	virtual void onButtonPressed( const MouseButtonEvent& );

private:
	
	WindowPtr window;
	InputMap* inputMap; 
	
	CameraPtr camera;
	ViewportPtr viewport;
	ViewportPtr viewport2;
	
	LabelPtr label;
	NodePtr nodeFBO;
	TexturePtr textureFBO;
	RenderBufferPtr fbo;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
