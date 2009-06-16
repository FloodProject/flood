/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>
#include <vapor/math/Math.h>
#include <al.h>
#include <alc.h>

using namespace vapor;

/**
 * Simple example showing how to render a triangle using VBOs.
 */

class Example : public Framework
{
	// Initializes the app.
	virtual void onInit();	

	// Sets up all the needed resources.
	virtual void onSetupResources();

	// Called to update stuff
	virtual void onUpdate();

	// Renders the scene
	virtual void onRender();

private:
  ALuint bufferID;            // The OpenAL sound buffer ID
  ALuint sourceID;            // The OpenAL sound source
};

#define VAPOR_EXAMPLE_NAME Example
#include <vapor/Main.h>
