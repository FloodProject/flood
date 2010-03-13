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

BEGIN_EVENT_TABLE(vaporControl, wxGLCanvas)
	EVT_IDLE(vaporControl::OnIdle)
    EVT_PAINT(vaporControl::OnPaint)
	EVT_SIZE(vaporControl::OnSize)
	EVT_KEY_DOWN(vaporControl::OnKeyDown)
	EVT_KEY_UP(vaporControl::OnKeyUp)
	EVT_MOUSE_EVENTS(vaporControl::OnMouseEvent)
	EVT_MOUSE_CAPTURE_LOST(vaporControl::OnMouseCaptureLost)
END_EVENT_TABLE()

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
	engine(engine), updatedOnce( false )
{
	InitControl();
}

//-----------------------------------//

void vaporControl::InitControl()
{
	if(!engine) return;
	render::Device* const device = engine->getRenderDevice();

	info("vaporEditor", "Creating a new wxWidgets control");

	// Create a new vapor3D window.
	wxSize size = GetSize();
	WindowSettings settings(size.GetX(), size.GetY());
	window = new vaporWindow(settings, this);

	// Setup the window as our main render target.
	device->setWindow( window );
	device->setRenderTarget( window );
	window->makeCurrent();

	// Setup input in the engine.
	inputManager = window->im;
	engine->setupInput();
}

//-----------------------------------//

void vaporControl::OnIdle(wxIdleEvent& event)
{
	//OnUpdate();

	// Asks wxWidgets to send more idle events.
	//event.RequestMore( true );
}

//-----------------------------------//

void vaporControl::OnUpdate()
{
	static int i = 0;
	if( i < 10 ) debug( "Update %d", i++ );

	updatedOnce = true;

	// Keep track of the frame times.
	lastFrameTime = frameTimer.getElapsedTime();
	frameTimer.reset();

	// Update the engine with the last frame time.
	engine->update( lastFrameTime );
}

//-----------------------------------//

void vaporControl::OnRender()
{
	// Prevent rendering without updating the scene once.
	if( !updatedOnce ) return;

	static int i = 0;
	if( i < 10 ) debug( "Render %d", i++ );

	const math::Color bg( 0.0f, 0.10f, 0.25f );

	render::Device* device = engine->getRenderDevice();
	
	device->setClearColor(bg);
	device->clearTarget();

	cam->render();
}

//-----------------------------------//

void vaporControl::OnPaint(wxPaintEvent& WXUNUSED(event))
{   
	// From the PaintEvent docs: "the application must always create
	// a wxPaintDC object, even if you do not use it."
	// http://docs.wxwidgets.org/trunk/classwx_paint_event.html
	wxPaintDC dc(this);

	OnRender();

	// Swaps the front and back buffers.
	window->update();
}

//-----------------------------------//

void vaporControl::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	window->processResize( event.GetSize() );
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
	if( event.ButtonDown( wxMOUSE_BTN_LEFT ) && !HasFocus() )
		SetFocus();
	
	inputManager->processMouseEvent( event );
}

//-----------------------------------//

void vaporControl::OnMouseCaptureLost(wxMouseCaptureLostEvent&)
{
	window->mouseCaptured = false;
	SetCursor( wxNullCursor );
}

//-----------------------------------//

void* vaporControl::getHandle()
{
	void* handle;

#ifdef __WXMSW__
	// Handle for Windows systems.
	HWND hwnd = (HWND) GetHandle();
	handle = hwnd;
#else
	// Any other unsupported system
	#error Not supported on this platform.
#endif

	return handle;
}

//-----------------------------------//

} } // end namespaces