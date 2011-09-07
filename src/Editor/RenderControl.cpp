/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "RenderControl.h"
#include "RenderWindow.h"
#include "EditorInputManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

enum Timers
{
	UPDATE_TIMER = wxID_HIGHEST + 321,
	RENDER_TIMER
};

BEGIN_EVENT_TABLE(RenderControl, wxGLCanvas)
	EVT_TIMER(UPDATE_TIMER, RenderControl::doUpdate)
	EVT_TIMER(RENDER_TIMER, RenderControl::doRender)
	EVT_PAINT(RenderControl::OnPaint)
	EVT_SIZE(RenderControl::OnSize)
	EVT_SET_FOCUS(RenderControl::OnFocusSet)
	EVT_KILL_FOCUS(RenderControl::OnFocusKill)
	EVT_KEY_DOWN(RenderControl::OnKeyDown)
	EVT_KEY_UP(RenderControl::OnKeyUp)
	EVT_MOUSE_EVENTS(RenderControl::OnMouseEvent)
	EVT_MOUSE_CAPTURE_LOST(RenderControl::OnMouseCaptureLost)
END_EVENT_TABLE()

const float MAX_RATE_UPDATE = 1.0f / 25;
const float MAX_RATE_RENDER = 1.0f / 60;

//-----------------------------------//

RenderControl::RenderControl( wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos, const wxSize& size,
	long style,	const wxString&	name, const wxPalette& )
	: wxGLCanvas(parent, id, attribList, pos, size, style | wxFULL_REPAINT_ON_RESIZE & (~wxCLIP_CHILDREN), name)
	, needsRedraw(false)
	, frameUpdateTimer(this, UPDATE_TIMER)
	, frameRenderTimer(this, RENDER_TIMER)
	, input(nullptr)
{
	LogInfo("Creating a new wxWidgets render control");

	// Create a new engine window.
	const wxSize& sz = GetSize();
	WindowSettings settings(sz.GetX(), sz.GetY());
	
	// Note: This will be deleted by the engine.
	window = AllocateThis(RenderWindow, settings, this);

	// Setup input in the engine.
	input = window->inputManager;
}

//-----------------------------------//

RenderControl::~RenderControl()
{
	LogDebug("Destroying RenderControl");
	Deallocate(window);
}

//-----------------------------------//

void RenderControl::startFrameLoop()
{
	frameUpdateTimer.Start(MAX_RATE_UPDATE);
	frameRenderTimer.Start(MAX_RATE_RENDER);
}

//-----------------------------------//

void RenderControl::stopFrameLoop()
{
	frameUpdateTimer.Stop();
	frameRenderTimer.Stop();
}

//-----------------------------------//

void RenderControl::doUpdate(wxTimerEvent&)
{
	onUpdate( MAX_RATE_RENDER );
}

//-----------------------------------//

void RenderControl::doRender(wxTimerEvent&)
{
	//if( !needsRedraw ) return;

	Refresh();
	needsRedraw = false;
}

//-----------------------------------//

void RenderControl::flagRedraw()
{
	needsRedraw = true;
}

//-----------------------------------//

void RenderControl::OnPaint(wxPaintEvent& WXUNUSED(event))
{   
	// TODO/HACK: wxWidgets won't invalidate the entire window region
	// on things like window overlap or tooltip hovering, so we need 
	// to force a complete redraw. This is ugly because I suspect it
	// draws twice so really try to find a better solution later.

	Refresh();

	// From the PaintEvent docs: "the application must always create
	// a wxPaintDC object, even if you do not use it."
	// http://docs.wxwidgets.org/trunk/classwx_paint_event.html
	wxPaintDC dc(this);

	onRender();
	
	// Swaps the front and back buffers.
	window->update();
}

//-----------------------------------//

void RenderControl::OnSize(wxSizeEvent& event)
{
	if( frameUpdateTimer.IsRunning() )
		onUpdate(0);
	
	window->processResize( event.GetSize() );
	flagRedraw();
}

//-----------------------------------//

void RenderControl::OnFocusSet(wxFocusEvent& event)
{
	window->handleWindowFocus(false);
}

//-----------------------------------//

void RenderControl::OnFocusKill(wxFocusEvent& event)
{
	window->handleWindowFocus(true);
}

//-----------------------------------//

void RenderControl::OnKeyDown(wxKeyEvent& event)
{
	input->processKeyEvent( event, true );
}

//-----------------------------------//

void RenderControl::OnKeyUp(wxKeyEvent& event)
{
	input->processKeyEvent( event, false );
}

//-----------------------------------//

void RenderControl::OnMouseEvent(wxMouseEvent& event)
{
	input->processMouseEvent( event );
}

//-----------------------------------//

void RenderControl::OnMouseCaptureLost(wxMouseCaptureLostEvent&)
{
	window->setCursorVisible(true);
	window->setCursorCapture(false);
}

//-----------------------------------//

NAMESPACE_EDITOR_END