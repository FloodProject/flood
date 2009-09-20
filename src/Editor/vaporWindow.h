/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Window.h"

#include "wx_InputManager.h"

#include <wx/glcanvas.h> 

/**
 * Window implementation using the wxWidgets GUI framework. This will be used 
 * to integrate vapor rendering into wxWidgets-based applications, thus making
 * possible to do custom tools, like world editors, model viewers, etc.
 */

class vaporWindow : public vapor::render::Window
{
public:

	vaporWindow(const vapor::render::WindowSettings& settings, wxGLCanvas* canvas);
	virtual ~vaporWindow();

	// Swaps the buffers (updates the display).
	virtual void update();

	// Shows/hides the window.
	virtual void show( bool hide = false );

	// Handle the window message events.
	virtual bool pumpEvents();

	// Sets the title of the window.
	virtual void setTitle(const std::string& title);

	// Sets the cursor visibility.
	virtual void setCursor(bool state);

	// Gets the input manager.
	virtual vapor::input::InputManager& getInputManager();

	// Makes this the current OpenGL context.
	virtual void makeCurrent();

private:

	// Opens a new window
	bool open();

	// SDL window id
	wxGLCanvas* canvas;

	// OpenGL context
	wxGLContext* context;

	// wxWidgets Input Manager
	wx_InputManager *im;
};

//} } // end namespaces

//#endif