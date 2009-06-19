/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

/**
 * Simple example showing how to render a triangle using VBOs.
 */

class Example : public vapor::Framework
{
	// Initializes the app.
	virtual void onInit();	

	// Sets up all the needed resources.
	virtual void onSetupResources();

	// Called to set up the scene
	virtual void onSetupScene();

	// Called to update stuff
	virtual void onUpdate();

	// Renders the scene
	virtual void onRender();

private:

};

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
