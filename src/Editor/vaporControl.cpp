/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "vaporControl.h"
#include "vaporWindow.h"

namespace vapor { namespace editor {

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

enum Timers
{
	UPDATE_TIMER = 5976,
	RENDER_TIMER
};

BEGIN_EVENT_TABLE(vaporControl, wxGLCanvas)
	EVT_TIMER(UPDATE_TIMER, vaporControl::doUpdate)
	EVT_TIMER(RENDER_TIMER, vaporControl::doRender)
    EVT_PAINT(vaporControl::OnPaint)
	EVT_SIZE(vaporControl::OnSize)
	EVT_KEY_DOWN(vaporControl::OnKeyDown)
	EVT_KEY_UP(vaporControl::OnKeyUp)
	EVT_MOUSE_EVENTS(vaporControl::OnMouseEvent)
	EVT_MOUSE_CAPTURE_LOST(vaporControl::OnMouseCaptureLost)
END_EVENT_TABLE()

const double MAX_RATE_UPDATE = 1.0f / 25;
const double MAX_RATE_RENDER = 1.0f / 60;

//-----------------------------------//

vaporControl::vaporControl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id,
					const int* attribList,
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString&	name,
					const wxPalette& WXUNUSED(pallete))
	: wxGLCanvas(parent, id, attribList, pos, size, style, name),
	needsRedraw( false ), frameUpdateTimer(this, UPDATE_TIMER),
	frameRenderTimer(this, RENDER_TIMER)

{
	if(!engine) return;
	render::Device* const device = engine->getRenderDevice();

	info("vaporEditor", "Creating a new wxWidgets control");

	// Create a new vapor3D window.
	const wxSize& sz = GetSize();
	WindowSettings settings(sz.GetX(), sz.GetY());
	
	// Note: This will be deleted by the engine.
	window = new vaporWindow(settings, this);

	// Setup the window as our main render target.
	device->setWindow( *window );
	device->setRenderTarget( *window );
	window->makeCurrent();

	// Setup input in the engine.
	inputManager = window->im;
	engine->setupInput();

	frameUpdateTimer.Start(MAX_RATE_UPDATE);
	frameRenderTimer.Start(MAX_RATE_RENDER);
}

//-----------------------------------//

void vaporControl::doUpdate(wxTimerEvent&)
{
	if( !onUpdate.empty() )
		onUpdate( MAX_RATE_UPDATE );
}

//-----------------------------------//

void vaporControl::doRender(wxTimerEvent&)
{
	if( needsRedraw )
	{
		Refresh();
		needsRedraw = false;
	}	
}

//-----------------------------------//

void vaporControl::flagRedraw()
{
	needsRedraw = true;
}

//-----------------------------------//

void vaporControl::OnPaint(wxPaintEvent& WXUNUSED(event))
{   
	// From the PaintEvent docs: "the application must always create
	// a wxPaintDC object, even if you do not use it."
	// http://docs.wxwidgets.org/trunk/classwx_paint_event.html
	wxPaintDC dc(this);

	if( !onRender.empty() )
		onRender();

	// Swaps the front and back buffers.
	window->update();
}

//-----------------------------------//

void vaporControl::OnSize(wxSizeEvent& event)
{
	window->processResize( event.GetSize() );
	flagRedraw();
}

//-----------------------------------//

void vaporControl::OnKeyDown(wxKeyEvent& event)
{
	inputManager->processKeyEvent( event, true );
}

//-----------------------------------//

void vaporControl::OnKeyUp(wxKeyEvent& event)
{
	inputManager->processKeyEvent( event, false );
}

//-----------------------------------//

void vaporControl::OnMouseEvent(wxMouseEvent& event)
{
	if( event.ButtonDown(wxMOUSE_BTN_LEFT) && !HasFocus() )
		SetFocus();
	
	inputManager->processMouseEvent( event );
}

//-----------------------------------//

void vaporControl::OnMouseCaptureLost(wxMouseCaptureLostEvent&)
{
	SetCursor( wxNullCursor );
}

//-----------------------------------//

} } // end namespaces