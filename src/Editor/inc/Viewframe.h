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

/**
 * Viewframe displays a view.
 */

class Viewframe : public wxPanel
{
public:

	Viewframe( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE );

	~Viewframe();

	// Switches to the default camera.
	void switchToDefaultCamera();

	SETTER(MainCamera, const CameraPtr&, mainCamera)
	GETTER(MainCamera, const CameraWeakPtr&, mainCamera)

	// Creates and adds a new view to the viewframe.
	RenderView* createView();

	// Flags the backing control for redraw.
	void flagRedraw();

	// Gets the associated view.
	GETTER(View, RenderView*, view)

	// Gets the associated control.
	GETTER(Control, RenderControl*, control)

	// Gets the sizer of this panel.
	GETTER(Sizer, wxSizer*, mainSizer)

protected:

	// Camera of the viewframe.
	CameraWeakPtr mainCamera;

	// Control where the scene will be drawn.
	RenderControl* control;

	// View associated with the control.
	RenderView* view;

	// Main sizer.
	wxSizer* mainSizer;
};

//-----------------------------------//

} } // end namespaces