/************************************************************************
*
* vaporEngine (2008-2009)
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
 * Represents the information that the user wants to pass to the engine
 * related to the creation of a new rendering window, like for example
 * bits-per-pixel, the size of the the window, if it is fullscreen, and 
 * in the future, anti-aliasing settings, and custom window handle hooks.
 */

class Settings
{
public:

	Settings(const int width, const int height, 
		const int bpp = 32, const bool fullscreen = false,
		void* customHandle = nullptr);

	// Gets the width of the window
	const int getWidth() const { return width; }
	
	// Gets the height of the window
	const int getHeight() const { return height; }
	
	// Gets the bits-per-pixel of the window
	const int getBpp() const { return bpp; }

	// Gets the custom handle of the window 
	void* getCustomHandle() const { return customHandle; }
	
	// Is this window fullscreen?
	const bool isFullscreen() const { return fullscreen; }

public:

	int width, height, bpp;
	bool fullscreen;
	void* customHandle;
};

/**
 * Represents an region on the screen that the renderer can send
 * the final representation of the scene, the rendered output image.
 * In most platforms (PCs) this will be just a normal window on the
 * desktop, but on some platforms (consoles, for instance) this might 
 * be slighty different, so some methods might not make much sense. 
 */

class Window : public RenderTarget
{
public:

	Window (Settings& settings);
	virtual ~Window ();

	// Updates the window content
	virtual void update() = 0;

	// Handles the event loop of the window
	virtual bool pumpEvents() = 0;

	// Sets the title of the window
	virtual void setTitle (const std::string& title) const = 0;

	// Sets the visibility of the mouse cursor 
	virtual void setCursor (bool state) const = 0;

	// Gets the window settings
	virtual Settings& getSettings();

protected:

	// Holds the window settings
	Settings settings;
};

} } // end namespaces
