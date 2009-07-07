/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#include "vapor/Platform.h"
#include "vapor/render/Window.h"

#include <wx/glcanvas.h> 

/**
 * Window implementation using the wxWidgets GUI framework. This will be used 
 * to integrate vapor rendering into wxWidgets-based applications, thus making
 * possible to do custom tools, like world editors, model viewers, etc.
 */

class vaporWindow : public vapor::render::Window
{
public:

	vaporWindow(vapor::render::Settings& settings, wxGLCanvas* canvas);
	virtual ~vaporWindow();

	// Swaps the buffers (updates the display)
	void update();

	// Handle the window message events
	bool pumpEvents();

	// Sets the title of the window
	void setTitle(const std::string& title) const;

	// Sets the cursor visibility
	void setCursor(bool state) const;

	// Makes this the current OpenGL context
	void makeCurrent();

private:

	// Opens a new window
	bool open();

	// SDL window id
	wxGLCanvas* canvas;

	// OpenGL context
	wxGLContext* context;
};

//} } // end namespaces

//#endif