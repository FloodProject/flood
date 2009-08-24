/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>

#include <vapor/input/Keyboard.h>

//-----------------------------------//

/**
 * Simple example.
 */

class Example : public vapor::Framework
{
public:

	Example();

protected:

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

	void onKeyPressed( const vapor::input::KeyEvent& );

private:

	float r,g,b;
	bool runLoop;
};

//-----------------------------------//

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
