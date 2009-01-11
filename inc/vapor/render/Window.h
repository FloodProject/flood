/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Target.h"

namespace vapor {
	namespace render {

class WindowSettings
{

public:

	WindowSettings(const int width, const int height, 
		const int bpp = 32, const bool fullscreen = false);

	int width, height, bpp;
	bool fullscreen;
};

class Window : public RenderTarget
{

public:

	Window (WindowSettings *windowSettings);
	virtual ~Window ();

	// Updates the window content
	virtual void update() = 0;

	// Handles the event loop of the window
	virtual bool pump () = 0;

	// Sets the title of the window
	virtual void setTitle (const std::string& title) const = 0;

	// Sets the visibility of the mouse cursor 
	virtual void setCursor (bool state) const = 0;

	virtual WindowSettings& getWindowSettings();

protected:

	WindowSettings *_windowSettings;
};

} } // end namespaces
