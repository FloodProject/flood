/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

class RenderControl;

//-----------------------------------//

/**
 * Viewport with camera control.
 */

class Viewframe : public wxPanel
{
public:

	Viewframe( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL );

	~Viewframe();

	// Creates and adds a new viewport to the viewframe.
	Viewport* createViewport( NodePtr node );

	// Flags the backing control for redraw.
	void flagRedraw();

	// Gets the associated camera.
	GETTER(Camera, const CameraPtr, viewport->getCamera())

	// Gets the associated control.
	GETTER(Control, RenderControl*, control)

	// Gets the associated viewport.
	GETTER(Viewport, Viewport*, viewport)

protected:

	void build();
	void updatePosition();

	// Event handlers.
	void OnToolChoice( wxCommandEvent& event );
	void onCameraTransform();
	void onText( wxCommandEvent& event );
	void onTextEnter( wxCommandEvent& event );
	void onCameraSpeedSpin( wxSpinDoubleEvent& event );
	
	// Control where the scene will be drawn.
	RenderControl* control;

	// Viewport associated with the control.
	Viewport* viewport;

	// UI controls.
	wxString X;
	wxString Y;
	wxString Z;

	wxTextCtrl* txt_X;
	wxTextCtrl* txt_Y;
	wxTextCtrl* txt_Z;

	wxSpinCtrlDouble* spn_CameraSpeed;
	
	wxBitmapButton* btn_Wireframe;
	wxBitmapButton* btn_Textures;
	
	wxChoice* choice_View;

private:

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces