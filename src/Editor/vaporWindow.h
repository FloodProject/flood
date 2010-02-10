/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/glcanvas.h> 
#include "vaporInputManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Window implementation using the wxWidgets GUI framework. This will be used 
 * to integrate vapor rendering into wxWidgets-based applications, thus making
 * it possible to do custom tools, like world editors, model viewers, etc.
 * This class is only the class that connects the wxWidgets widget, with the
 * vapor engine, so there is another class, vaporControl, that handles the real
 * hard work :).
 */

class vaporWindow : public vapor::render::Window
{
	friend class vaporControl;

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
	virtual void setCursorVisible(bool state);

	// Gets the cursor visibility.
	virtual bool isCursorVisible() const;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y );

	// Gets the input manager.
	virtual input::InputManager& getInputManager();

	// Makes this the current OpenGL context.
	virtual void makeCurrent();

private:

	// Processes a window resize.
	void processResize(const wxSize& size);

	// Opens a new window
	bool open();

	// OpenGL Canvas Widget
	wxGLCanvas* canvas;

	// OpenGL context
	wxGLContext* context;

	// wxWidgets Input Manager
	vaporInputManager *im;

	// Is the mouse cursor captured?
	bool mouseCaptured;
};

//-----------------------------------//

} } // end namespaces