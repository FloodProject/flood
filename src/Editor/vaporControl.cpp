/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vaporControl.h"

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

//IMPLEMENT_CLASS (vaporControl, wxControl)

//-----------------------------------//

vaporControl::vaporControl(wxWindow* parent, wxWindowID id,
				const wxPoint& pos,
				const wxSize& siz,
				long style,
				const wxValidator& val,
				const wxString& name)
	: wxControl(parent, id, pos, siz, style, val, name)
{


}


//-----------------------------------//

vaporControl::~vaporControl()
{

}

//-----------------------------------//

void vaporControl::initControl()
{
	if(!engine) return;

	render::Device* device = engine->getRenderDevice();

	info("vaporEditor", "Creating a new wxWidgets control");

	// lets get a window handle to pass to vapor
	int handle = getHandle();

	// get the control size
	wxSize size = GetSize();

	// construct a settings object to pass to vapor
	Settings settings(size.GetX(), size.GetY(), 32, false, handle);

	Window& window = device->createWindow(settings);
	device->setRenderTarget(&window);
	this->window = &window;
}

//-----------------------------------//

int vaporControl::getHandle()
{
	int handle;

#ifdef __WXMSW__
	// Handle for Windows systems
	HWND hwnd = (HWND) GetHandle();
	handle = (int) hwnd;
#else
	// Any other unsupported system
	#error Not supported on this platform.
#endif

	return handle;
}

//-----------------------------------//