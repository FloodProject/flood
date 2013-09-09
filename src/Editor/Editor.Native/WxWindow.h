/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "WxPlatform.h"
#include "Engine/Window/Window.h"

class WxGLContext;
class WxGLCanvas;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

/**
 * Window implementation using the wxWidgets GUI framework (wxGLCanvas).
 * This will be used to integrate engine rendering into wxWidgets-based
 * applications, thus making it possible to do custom tools, like world
 * editors, model viewers, etc. This class only implements the Window
 * interface on the engine side. There is another class, WxRenderContext,
 * that handles the rest of the work.
 */

class WxWindow : public Window
{
	friend class WxGLCanvas;

public:

	WxWindow(const WindowSettings& settings, wxWindow* window,
		WxInputManager* inputManager);
	virtual ~WxWindow();

	// Creates a new render context.
	RenderContext* createContext(const RenderContextSettings&) OVERRIDE;

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
	void setTitle(const String& title) OVERRIDE;

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

	// Processes a window resize.
	void processResize(const wxSize& size);

	// Processes a window idle event.
	void processIdle(wxIdleEvent& event);

	// wxWidgets window.
	wxWindow* window;

	// Input manager.
	WxInputManager* inputManager;

	// wxWidgets OpenGL canvas.
	WxGLCanvas* canvas;

	// Cursor visiblity.
	bool cursorVisible;

	// Cursor priority.
	int32 cursorPriority;
};

//-----------------------------------//

NAMESPACE_EDITOR_END