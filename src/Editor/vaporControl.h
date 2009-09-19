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

/**
 * Let's link vapor with wxWidgets. There are some different approaches
 * that can be used to do this, some more flexible than others. We could
 * create a custom wxWidgets window impementation in vapor, for example,
 * but this class approaches the problem by getting just an OS-specific
 * window handle, and letting vaporEngine do all the dirty work of setting
 * up the rendering context and implementing the window class. Let's see
 * if this approach works as well as I think it won't. :(
 */

class vaporControl : public wxGLCanvas 
{
public:

	vaporControl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id = wxID_ANY,
					const int* attribList = nullptr,
					const wxPoint& pos	= wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = 0,
					const wxString&	name = "vaporGLCanvas",
					const wxPalette& palette = wxNullPalette); 	

	virtual ~vaporControl();

	// Add your frame updating code here.
	void OnUpdate();

	// Called then the app is idle. We will refresh the widget here
	// to ensure maximum framerate is achieved.
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);

//-------//

	vapor::Engine* getEngine() { return engine; }
	void setEngine(vapor::Engine* engine) { this->engine = engine; }

protected:

	void initControl();

	// Returns the window handle of this control
	void* getHandle();

	// window associated this control
	vapor::render::Window* window;

	// holds the main vapor engine
	vapor::Engine* engine;

	DECLARE_EVENT_TABLE()
};
