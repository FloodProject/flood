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
#include "Render/Window.h"

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
	void update() OVERRIDE;

	// Shows/hides the window.
	void show( bool hide = false ) OVERRIDE;

	// Handle the window message events.
	bool pumpEvents() OVERRIDE;

	// Makes this the current OpenGL context.
	void makeCurrent() OVERRIDE;

	// Gets if the window has focus.
	bool hasFocus() OVERRIDE;

	// Sets the title of the window.
	void setTitle(const std::string& title) OVERRIDE;

	// Sets the cursor visibility.
	void setCursorVisible(bool state) OVERRIDE;

	// Gets the cursor visibility.
	bool isCursorVisible() const OVERRIDE;

	// Sets the capture of the mouse.
	void setCursorCapture( bool state ) OVERRIDE;

	// Gets the cursor position on screen.
	Vector2i getCursorPosition() const OVERRIDE;

	// Sets the cursor position on screen.
	void setCursorPosition( int x, int y ) OVERRIDE;

	// Sets the cursor visible if the priority matches.
	void setCursorVisiblePriority(bool state, int32 priority);

	// Gets the cursor priority.
	GETTER(CursorPriority, int32, cursorPriority)

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
	EditorInputManager* inputManager;

	// Cursor visiblity.
	bool cursorVisible;

	// Cursor priority.
	int32 cursorPriority;
};

//-----------------------------------//

} } // end namespaces