/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Target.h"

namespace vapor {
	namespace render {

/**
 * This is the class used to pass the Window-related information to the engine.
 * Settings it can handle are Bits-per-pixel, the size of the the window,
 * if it is fullscreen, and in the future, if the user wants anti-aliasing.
 */

class Settings
{
public:

	Settings(const int width, const int height, 
		const int bpp = 32, const bool fullscreen = false);

	// Gets the width of the window
	const int width() const { return _width; }
	
	// Gets the height of the window
	const int height() const { return _height; }
	
	// Gets the bits-per-pixel of the window
	const int bpp() const { return _bpp; }
	
	// Is this window fullscreen?
	const bool fullscreen() const { return _fullscreen; }

protected:

	int _width, _height, _bpp;
	bool _fullscreen;
};

/**
 * Represents an region on the screen that the Renderer can send
 * the final representation of the scene, or the engine's output.
 * In most platforms (PCs) this will be just a normal window on the
 * desktop, but on some platforms (consoles, for instance) this might 
 * mean the full screen, so some methods might not make much sense. 
 */

class Window : public RenderTarget
{
public:

	Window (Settings& settings);
	virtual ~Window ();

	// Updates the window content
	virtual void update() = 0;

	// Handles the event loop of the window
	virtual bool pump () = 0;

	// Sets the title of the window
	virtual void setTitle (const string& title) const = 0;

	// Sets the visibility of the mouse cursor 
	virtual void setCursor (bool state) const = 0;

	// Gets the window settings
	virtual Settings& getSettings();

protected:

	// Holds the window settings
	Settings* settings;
};

} } // end namespaces
