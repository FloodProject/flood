/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <wx/glcanvas.h>

#include "Core/Event.h"

FWD_DECL(Engine)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class RenderTarget;
class RenderWindow;
class EditorInputManager;

class WxRenderContext : public RenderContext
{
public:

	WxRenderContext(wxGLCanvas* canvas);
	virtual ~WxRenderContext();

	void makeCurrent(RenderTarget* target) OVERRIDE;

	// OpenGL context.
	wxGLContext* contextGL;
};

class RenderControl : public wxGLCanvas
{
public:

	RenderControl( wxWindow* parent, wxWindowID id = wxID_ANY,
					const int* attribList = nullptr,
					const wxPoint& pos	= wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE,
					const wxString&	name = "RenderGLCanvas" );

	~RenderControl();

	// Flag this control to be redrawn.
	void flagRedraw();

	// Starts firing the update and render frame timers.
	void startFrameLoop();

	// Starts firing the update and render frame timers.
	void stopFrameLoop();

	// Gets the associated window.
	GETTER(RenderWindow, RenderWindow*, window)

	// Gets the associated input manager.
	GETTER(InputManager, EditorInputManager*, input)

	// Add your frame updating code here.
	Delegate1<float> onUpdate;

	// Add your frame rendering code here.
	Delegate0<> onRender;

protected:

	// Handles the update and redraw logic.
	void doUpdate(wxTimerEvent&);
	void doRender(wxTimerEvent&);

	// Window events.
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnFocusSet(wxFocusEvent& event);
	void OnFocusKill(wxFocusEvent& event);

	// Input events.
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

	// Timer responsible for update ticks.
	wxTimer frameUpdateTimer;

	// Timer responsible for rendering ticks.
	wxTimer frameRenderTimer;

	// Window associated with this control.
	RenderWindow* window;

	// Holds an instance of the input manager.
	EditorInputManager* input;

	// Tracks if the control needs to be redrawn.
	bool needsRedraw;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

NAMESPACE_EDITOR_END