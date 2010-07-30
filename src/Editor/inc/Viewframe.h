/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

class RenderControl;

//-----------------------------------//

/**
 * Viewframe displays a viewport.
 */

class Viewframe : public wxPanel
{
public:

	Viewframe( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE );

	~Viewframe();

	// Creates and adds a new viewport to the viewframe.
	Viewport* createViewport( NodePtr node );

	// Flags the backing control for redraw.
	void flagRedraw();

	// Gets the associated viewport.
	GETTER(Viewport, Viewport*, viewport)

	// Gets the associated control.
	GETTER(Control, RenderControl*, control)

	// Gets the sizer of this panel.
	GETTER(Sizer, wxSizer*, mainSizer)

protected:

	// Control where the scene will be drawn.
	RenderControl* control;

	// Viewport associated with the control.
	Viewport* viewport;

	// Main sizer.
	wxSizer* mainSizer;
};

//-----------------------------------//

} } // end namespaces