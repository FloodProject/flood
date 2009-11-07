/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
// for all others, include the necessary headers (this file is usually all 
//  you need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <vapor/Engine.h>

#include <wx/glcanvas.h>

namespace vapor { namespace editor {

class vaporWindow;

//-----------------------------------//

/**
 * Let's link vapor with wxWidgets. There are some different approaches
 * that can be used to do this, some more flexible than others. We could
 * create a custom wxWidgets window impementation in vapor, for example,
 * but this class approaches the problem by getting just an OS-specific
 * window handle, and letting vaporEngine do all the dirty work of setting
 * up the rendering context and implementing the window class. Let's see
 * if this approach works as well as I think it won't. :(
 * Update: SDL didn't work with external window handles, I had to patch
 * it myself, so I ended up creating what I said above, a specific window
 * implementation for vapor based on wxGLCanvas. Now we use SFML instead
 * of SDL, and it has support for external window handles, so it might
 * work well with the approach I tried first with SDL. Can't be bothered
 * to try it now, because the wxGLCanvas-based approach is working good.
 */

class vaporControl : public wxGLCanvas 
{
public:

	vaporControl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id = wxID_ANY,
					const int* attribList = nullptr,
					const wxPoint& pos	= wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = 0 | wxFULL_REPAINT_ON_RESIZE,
					const wxString&	name = "vaporGLCanvas",
					const wxPalette& palette = wxNullPalette); 	

	virtual ~vaporControl();

	// Add your frame updating code here.
	void OnUpdate();

	// Called then the app is idle. We will refresh the widget here
	// to ensure maximum framerate is achieved.
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	// Gets the associated instance of the vaporEngine.
	vapor::Engine* getEngine() { return engine; }

protected:

	// Sets the associated instance of the vaporEngine.
	void setEngine(vapor::Engine* engine) { this->engine = engine; }

	// Initializes the control.
	void initControl();

	// Returns the window handle of this control.
	void* getHandle();

	// vaporEngine's Window associated with this control.
	vaporWindow* window;

	// Holds an instance to the vaporEngine.
	vapor::Engine* engine;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces