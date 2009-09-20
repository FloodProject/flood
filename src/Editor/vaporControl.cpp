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

#ifdef __WXGTK__
	#include <gdk/gdk.h>
	#include <gtk/gtk.h>
	#include <gdk/gdkx.h>
	#include <wx/gtk/win_gtk.h>
	#include <GL/glx.h>
#endif

using namespace vapor;
using namespace vapor::render;
using namespace vapor::math;

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(vaporControl, wxGLCanvas)
    EVT_IDLE(vaporControl::OnIdle)
    EVT_PAINT(vaporControl::OnPaint)
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
	: wxGLCanvas(parent, id, attribList, pos, size, style, name),
		engine(engine)
{
	initControl();
}

//-----------------------------------//

vaporControl::~vaporControl()
{
	delete window;
}

//-----------------------------------//

void vaporControl::OnUpdate()
{
	render::Device* device = engine->getRenderDevice();
	
	device->setRenderTarget(window);
	device->setClearColor(Colors::White);
	device->clearTarget();
}

//-----------------------------------//

void vaporControl::OnPaint(wxPaintEvent& event)
{
	OnUpdate();

	window->update();

	wxGLCanvas::OnPaint(event);
}

//-----------------------------------//

void vaporControl::OnIdle(wxIdleEvent& WXUNUSED(event))
{
	OnUpdate();

	window->update();
	Refresh();
}

//-----------------------------------//

void vaporControl::initControl()
{
	if(!engine) return;

	render::Device* device = engine->getRenderDevice();

	info("vaporEditor", "Creating a new wxWidgets control");

	// get the control size
	wxSize size = GetSize();

	// construct a settings object to pass to vapor
	WindowSettings settings(size.GetX(), size.GetY());

	Window* window = new vaporWindow(settings, this);
	device->setRenderTarget(window);
	this->window = window;
}

//-----------------------------------//

void* vaporControl::getHandle()
{
	void* handle;

#ifdef __WXMSW__
	// Handle for Windows systems
	HWND hwnd = (HWND) GetHandle();
	handle = hwnd;
#else
	// Any other unsupported system
	#error Not supported on this platform.
#endif

	return handle;
}

//-----------------------------------//