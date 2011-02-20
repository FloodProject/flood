/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

FWD_DECL_SHARED_WEAK(Camera)
FWD_DECL(RenderView)

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
		long style = wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE );

	~Viewframe();

	// Switches to the default camera.
	void switchToDefaultCamera();

	// Gets the main camera of the viewframe.
	GETTER(MainCamera, const CameraWeakPtr&, mainCamera)

	// Sets the main camera of the viewframe.
	SETTER(MainCamera, const CameraPtr&, mainCamera)

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

	// Gets called when camera changes in the view.
	void onCameraChanged(const CameraPtr& camera);

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