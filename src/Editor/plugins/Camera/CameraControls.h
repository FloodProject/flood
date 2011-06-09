/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "CameraPlugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

class Viewframe;
class EditorFrame;

typedef std::map<std::string, CameraPtr> CameraMap;

/**
 * Widget providing advanced camera controls.
 */

class CameraControls : public wxPanel
{
	friend CameraPlugin;

public:

	CameraControls( EditorFrame* editor,
		wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL );

	// Updates the camera position.
	void updateCameraPosition();

	// Updates the camera speed value.
	void updateCameraSpeedSpin();

	// Updates the camera selection.
	void updateCameraSelection();

	// Sets the current camera.
	void setCamera(const CameraPtr& camera);

protected:

	// Builds the GUI controls into the panel.
	void buildControls();

	// Handles the view changing.
	void onCameraTransform();

	// Gets the main camera.
	TransformPtr getCameraTransform() const;

	// Gets the cameras in the scene.
	void getCamerasInScene(const ScenePtr& scene);

	// Event handlers.
	void onToolChoice( wxCommandEvent& event );
	void onText( wxCommandEvent& event );
	void onTextEnter( wxCommandEvent& event );
	void onCameraSpeedSpin( wxSpinDoubleEvent& event );
	void onCameraChoice( wxCommandEvent& event );

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

	Viewframe* viewframe;
	EditorFrame* editor;

	CameraMap cameras;
	CameraPtr currentCamera;
};

//-----------------------------------//

} } // end namespaces