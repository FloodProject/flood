/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "API.h"
#include "WxRenderContext.h"
#include "WxWindow.h"
#include "WxPlatform.h"
#include "Graphics/RenderDevice.h"

#include <wx/glcanvas.h>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

WxRenderContext::WxRenderContext(wxGLCanvas* canvas)
	: contextGL(nullptr)
{
	// Create the OpenGL context.
	contextGL = new wxGLContext(canvas);
	
	if( !contextGL )
	{
		LogError("Error creating wxGLCanvas context");
		return;
	}
}

//-----------------------------------//

WxRenderContext::~WxRenderContext()
{
	LogDebug("Destroying OpenGL Context");
	Deallocate(contextGL);
}

//-----------------------------------//

void WxRenderContext::makeCurrent(RenderTarget* target)
{
	WxWindow* window = (WxWindow*) target->getUserData();
	if (!window) return;

	wxGLCanvas* canvas = window->canvas;
	assert(canvas && "Expected a valid wxGLCanvas instance");

	contextGL->SetCurrent(*canvas);

	setTarget(target);
	GetRenderDevice()->setActiveContext(this);
}

//-----------------------------------//

enum Timers
{
	UPDATE_TIMER = wxID_HIGHEST + 321,
	RENDER_TIMER
};

BEGIN_EVENT_TABLE(WxGLCanvas, wxGLCanvas)
	EVT_TIMER(UPDATE_TIMER, WxGLCanvas::doUpdate)
	EVT_TIMER(RENDER_TIMER, WxGLCanvas::doRender)
	EVT_PAINT(WxGLCanvas::OnPaint)
	EVT_SIZE(WxGLCanvas::OnSize)
	EVT_SET_FOCUS(WxGLCanvas::OnFocusSet)
	EVT_KILL_FOCUS(WxGLCanvas::OnFocusKill)
	EVT_KEY_DOWN(WxGLCanvas::OnKeyDown)
	EVT_KEY_UP(WxGLCanvas::OnKeyUp)
	EVT_CHAR(WxGLCanvas::OnChar)
	EVT_MOUSE_EVENTS(WxGLCanvas::OnMouseEvent)
	EVT_MOUSE_CAPTURE_LOST(WxGLCanvas::OnMouseCaptureLost)
END_EVENT_TABLE()

const float MAX_RATE_UPDATE = 1.0f / 25;
const float MAX_RATE_RENDER = 1.0f / 60;

//-----------------------------------//

WxGLCanvas::WxGLCanvas( wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name )
	: wxGLCanvas(parent, id, attribList, pos, size, style | (wxFULL_REPAINT_ON_RESIZE & (~wxCLIP_CHILDREN)) | wxWANTS_CHARS, name)
	, needsRedraw(false)
	, frameUpdateTimer(this, UPDATE_TIMER)
	, frameRenderTimer(this, RENDER_TIMER)
	, input(nullptr)
{
	LogInfo("Creating WxGLCanvas");

	SetBackgroundStyle(wxBG_STYLE_PAINT);
	window = (WxWindow*) parent->GetClientData();
	input = window->inputManager;
}

//-----------------------------------//

WxGLCanvas::~WxGLCanvas()
{
	LogDebug("Destroying WxGLCanvas");
}

//-----------------------------------//

void WxGLCanvas::startFrameLoop()
{
	frameUpdateTimer.Start(MAX_RATE_UPDATE);
	frameRenderTimer.Start(MAX_RATE_RENDER);
}

//-----------------------------------//

void WxGLCanvas::stopFrameLoop()
{
	frameUpdateTimer.Stop();
	frameRenderTimer.Stop();
}

//-----------------------------------//

void WxGLCanvas::doUpdate(wxTimerEvent&)
{
	//onUpdate( MAX_RATE_RENDER );
}

//-----------------------------------//

void WxGLCanvas::doRender(wxTimerEvent&)
{
	//if( !needsRedraw ) return;

	Refresh();
	needsRedraw = false;
}

//-----------------------------------//

void WxGLCanvas::flagRedraw()
{
	needsRedraw = true;
}

//-----------------------------------//

void WxGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	// From the PaintEvent docs: "the application must always create
	// a wxPaintDC object, even if you do not use it."
	// http://docs.wxwidgets.org/trunk/classwx_paint_event.html
	wxPaintDC dc(this);

	window->onRender();

	if( frameRenderTimer.IsRunning() )
	{
		//onRender();
	
		// Swaps the front and back buffers.
		//window->update();
	}
}

//-----------------------------------//

void WxGLCanvas::OnSize(wxSizeEvent& event)
{
	window->processResize( event.GetSize() );
	flagRedraw();
}

//-----------------------------------//

void WxGLCanvas::OnFocusSet(wxFocusEvent& event)
{
	window->handleWindowFocus(false);
}

//-----------------------------------//

void WxGLCanvas::OnFocusKill(wxFocusEvent& event)
{
	window->handleWindowFocus(true);
}

//-----------------------------------//

void WxGLCanvas::OnKeyDown(wxKeyEvent& event)
{
	input->processKeyEvent(&input->keyboard, event, true);
	event.Skip();
}

//-----------------------------------//

void WxGLCanvas::OnKeyUp(wxKeyEvent& event)
{
	input->processKeyEvent(&input->keyboard, event, false);
}

//-----------------------------------//

void WxGLCanvas::OnChar(wxKeyEvent& event)
{
	input->processCharEvent(&input->keyboard, event);
}

//-----------------------------------//

void WxGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
	input->processMouseEvent(&input->mouse, event);
}

//-----------------------------------//

void WxGLCanvas::OnMouseCaptureLost(wxMouseCaptureLostEvent&)
{
	window->setCursorVisible(true);
	window->setCursorCapture(false);
}

//-----------------------------------//

NAMESPACE_EDITOR_END