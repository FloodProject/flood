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
	: wxGLCanvas(parent, id, attribList, pos, size, style, name), engine(engine)
{
	InitControl();
	im = window->im;
	engine->setupInput();
}

//-----------------------------------//

vaporControl::~vaporControl()
{
	//delete window;
}

//-----------------------------------//

void vaporControl::InitControl()
{
	if(!engine) return;

	render::Device* device = engine->getRenderDevice();

	info("vaporEditor", "Creating a new wxWidgets control");

	// Get the size of the control.
	wxSize size = GetSize();

	// Construct a Settings object to pass to vapor.
	WindowSettings settings(size.GetX(), size.GetY());

	window = new vaporWindow(settings, this);

	device->setWindow( window );
	device->setRenderTarget( window );

	window->makeCurrent();
}

//-----------------------------------//

void vaporControl::OnMouseCaptureLost(wxMouseCaptureLostEvent& WXUNUSED(event))
{
	window->mouseCaptured = false;
	SetCursor( wxNullCursor );
}

//-----------------------------------//

void vaporControl::OnUpdate()
{
	engine->update( lastFrameTime );
}

//-----------------------------------//

void vaporControl::OnRender()
{
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

void vaporControl::OnIdle(wxIdleEvent& WXUNUSED(event))
{
	lastFrameTime = frameTimer.getElapsedTime();
	frameTimer.reset();
	OnUpdate();
	Refresh();
}

//-----------------------------------//

void vaporControl::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	//debug( "new size: %d %d", size.GetX(), size.GetY() );

	window->processResize( event.GetSize() );
}

//-----------------------------------//

void vaporControl::OnKeyDown(wxKeyEvent& event)
{
	im->processKeyEvent( event, true );
}

//-----------------------------------//

void vaporControl::OnKeyUp(wxKeyEvent& event)
{
	im->processKeyEvent( event, false );
}

//-----------------------------------//

void vaporControl::OnMouseEvent(wxMouseEvent& event)
{
	if( event.ButtonDown( wxMOUSE_BTN_LEFT ) && !HasFocus() )
		SetFocus();
	
	im->processMouseEvent( event );
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

Engine* vaporControl::getEngine()
{
	return engine;
}

//-----------------------------------//

void vaporControl::setCamera(const scene::CameraPtr& cam)
{
	this->cam = cam;
}

//-----------------------------------//

void vaporControl::setEngine(Engine* engine)
{
	this->engine = engine;
}

//-----------------------------------//

} } // end namespaces