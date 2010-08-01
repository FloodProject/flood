/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

class Viewframe;

//-----------------------------------//

/**
 * Widget providing advanced camera controls.
 */

class CameraControls : public wxPanel
{
public:

	CameraControls( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL );

	virtual ~CameraControls();

protected:

	// Builds the GUI controls into the panel.
	void buildControls();

	// Updates the camera position.
	void updateCameraPosition();

	// Updates the camera speed value.
	void updateCameraSpeedSpin();

	// Handles the view changing.
	void onCameraTransform();

	// Gets the main camera.
	TransformPtr getCameraTransform() const;

	// Event handlers.
	void onToolChoice( wxCommandEvent& event );
	void onText( wxCommandEvent& event );
	void onTextEnter( wxCommandEvent& event );
	void onCameraSpeedSpin( wxSpinDoubleEvent& event );

	Viewframe* viewframe;

	// UI controls.
	wxString X;
	wxString Y;
	wxString Z;

	wxTextCtrl* textX;
	wxTextCtrl* textY;
	wxTextCtrl* textZ;
	
	wxSpinCtrlDouble* spinCameraSpeed;
	wxBitmapButton* buttonWireframe;
	wxBitmapButton* buttonTextures;
	wxChoice* choiceView;
};

//-----------------------------------//

} } // end namespaces