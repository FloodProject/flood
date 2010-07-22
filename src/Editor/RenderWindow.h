/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/glcanvas.h> 
#include "EditorInputManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Window implementation using the wxWidgets GUI framework (wxGLCanvas).
 * This will be used to integrate vapor rendering into wxWidgets-based
 * applications, thus making it possible to do custom tools, like world
 * editors, model viewers, etc. This class only implements the Window
 * interface on the engine side. There is another class, RenderControl,
 * that handles the rest of the work.
 */

class RenderWindow : public Window
{
	friend class RenderControl;

public:

	RenderWindow(const WindowSettings&, wxGLCanvas* const);
	virtual ~RenderWindow();

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

	// Gets the cursor position on screen.
	virtual Vector2i getCursorPosition() const;

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y );

	// Gets the input manager.
	InputManager* getInputManager() { return im; }

	// Makes this the current OpenGL context.
	virtual void makeCurrent();

private:

	// Processes a window resize.
	void processResize(const wxSize& size);

	// Creates a new OpenGL context.
	bool createContext();

	// OpenGL Canvas Widget.
	wxGLCanvas* canvas;

	// OpenGL context.
	wxGLContext* context;

	// wxWidgets Input Manager.
	EditorInputManager* im;
};

//-----------------------------------//

} } // end namespaces