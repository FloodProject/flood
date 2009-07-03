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

/**
 * Let's link vapor with wxWidgets. There are some different approaches
 * that can be used to do this, some more flexible than others. We could
 * create a custom wxWidgets window impementation in vapor, for example,
 * but this class approaches the problem by getting just an OS-specific
 * window handle, and letting vaporEngine do all the dirty work of setting
 * up the rendering context and implementing the window class. Let's see
 * if this approach works as well as I think it won't. :(
 */

class vaporControl : public wxControl 
{
public:
	vaporControl(wxWindow* parent, wxWindowID id = wxID_ANY,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxSUNKEN_BORDER | wxTRANSPARENT_WINDOW,
					const wxValidator& val = wxDefaultValidator,
					const wxString& name = "vaporControl");

	virtual ~vaporControl();

	vapor::Engine* getEngine() { return engine; }
	void setEngine(vapor::Engine* engine) { this->engine = engine; }

	void initControl();

	// Returns the window handle of this control
	int getHandle();

protected:

	// window associated this control
	vapor::render::Window* window;

	// holds the main vapor engine
	vapor::Engine* engine;
};
