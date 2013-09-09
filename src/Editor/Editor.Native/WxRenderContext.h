/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "Core/Event.h"
#include "Graphics/RenderContext.h"

#include <wx/glcanvas.h>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class RenderTarget;
class WxWindow;
class WxInputManager;

class WxRenderContext : public RenderContext
{
public:

	WxRenderContext(wxGLCanvas* canvas);
	virtual ~WxRenderContext();

	void makeCurrent(RenderTarget* target) OVERRIDE;

	// OpenGL context.
	wxGLContext* contextGL;
};

class WxGLCanvas : public wxGLCanvas
{
public:

	WxGLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
					const int* attribList = nullptr,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE,
					const wxString& name = wxGLCanvasName);

	~WxGLCanvas();

	// Flag this control to be redrawn.
	void flagRedraw();

	// Starts firing the update and render frame timers.
	void startFrameLoop();

	// Starts firing the update and render frame timers.
	void stopFrameLoop();

	// Holds an instance of the input manager.
	WxInputManager* input;

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
	WxWindow* window;

	// Tracks if the control needs to be redrawn.
	bool needsRedraw;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

NAMESPACE_EDITOR_END