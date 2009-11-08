/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vaporControl.h"
#include "vaporWindow.h"

#include "vapor/render/Window.h"

#define VAPOR_USE_NAMESPACES
#include <vapor/Headers.h>

#ifdef __WXGTK__
	#include <gdk/gdk.h>
	#include <gtk/gtk.h>
	#include <gdk/gdkx.h>
	#include <wx/gtk/win_gtk.h>
	#include <GL/glx.h>
#endif

namespace vapor { namespace editor {

//-----------------------------------//

using namespace vapor::render;
using namespace vapor::math;

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(vaporControl, wxGLCanvas)
    EVT_IDLE(vaporControl::OnIdle)
    EVT_PAINT(vaporControl::OnPaint)
	EVT_SIZE(vaporControl::OnSize)
	EVT_KEY_DOWN(vaporControl::OnKeyDown)
	EVT_KEY_UP(vaporControl::OnKeyUp)
END_EVENT_TABLE()

//-----------------------------------//

vaporControl::vaporControl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id,
					const int* attribList,
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString&	name,
					const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name), engine(engine)
{
	initControl();
	im = window->im;
}

//-----------------------------------//

void vaporControl::initControl()
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

vaporControl::~vaporControl()
{
	//delete window;
}

//-----------------------------------//

void vaporControl::OnUpdate()
{
	engine->getSceneManager()->update( 0.001f );
}

//-----------------------------------//

void vaporControl::OnRender()
{
	static math::Color bg( 0.0f, 0.10f, 0.25f );
	
	render::Device* device = engine->getRenderDevice();
	
	device->setClearColor(bg);
	device->clearTarget();

	ScenePtr scene = engine->getSceneManager();
	CameraPtr cam = scene->getEntity("MainCamera")->getComponent<Camera>("Camera");

	cam->render();
}

//-----------------------------------//

void vaporControl::OnPaint(wxPaintEvent& event)
{
	// Same as SetCurrent(wxGLContext)
	window->makeCurrent();
    
	// From the PaintEvent docs: "the application must always create
	// a wxPaintDC object, even if you do not use it."
	// http://docs.wxwidgets.org/trunk/classwx_paint_event.html
	wxPaintDC dc(this);
	
	OnRender();

	window->update();
}

//-----------------------------------//

void vaporControl::OnIdle(wxIdleEvent& WXUNUSED(event))
{
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