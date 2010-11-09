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

	void onPageLoading(const PageEvent& event);

protected:

	// Initializes the app.
	virtual void onInit();	

	// Sets up all the needed resources.
	virtual void onSetupResources();

	// Called to set up the scene.
	virtual void onSetupScene();

	// Called to update stuff.
	virtual void onUpdate( double delta );

	// Renders the scene.
	virtual void onRender();

	// Callback functions.
	virtual void onKeyPressed( const KeyEvent& );
	virtual void onButtonPressed( const MouseButtonEvent& );

private:
	
	Window* window;
	RenderView* view;
	CameraPtr camera;

	LabelPtr labelFPS;
	TerrainPtr nodeTerrain;

	NodePtr nodeFBO;
	RenderView* viewportFBO;
	TexturePtr textureFBO;
	RenderBuffer* bufferFBO;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
