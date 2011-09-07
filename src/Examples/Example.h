/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Framework.h"
#include "Engine/Headers.h"

//-----------------------------------//

class Example : public Framework
{
public:

	Example(const char** argv = nullptr);
	~Example();

protected:

	// Initializes the app.
	void onInit() OVERRIDE;	

	// Sets up all the needed resources.
	void onSetupResources() OVERRIDE;

	// Called to set up the scene.
	void onSetupScene() OVERRIDE;

	// Called to update stuff.
	void onUpdate( float delta ) OVERRIDE;

	// Renders the scene.
	void onRender() OVERRIDE;

	// Callback functions.
	void onKeyPressed( const KeyEvent& ) OVERRIDE;
	void onButtonPressed( const MouseButtonEvent& ) OVERRIDE;

	RenderView* view;
	CameraPtr camera;
};