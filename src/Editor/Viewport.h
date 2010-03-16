/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vaporControl.h"

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Viewport with camera control.
 */

class Viewport : public wxPanel
{
public:

	Viewport( vapor::Engine* engine, wxWindow* parent, 
		wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL );

	// Each viewport has an associated camera.
	CameraPtr camera;
	NodePtr cameraNode;
	TransformPtr cameraTransform;
	
	// Control where the scene will be drawn.
	vaporControl* vaporCtrl;

protected:

	void build();
	void createCamera( Engine* );
	void updatePosition();

	// Event handlers.
	void OnToolChoice( wxCommandEvent& event );
	void onCameraTransform();
	void onText( wxCommandEvent& event );
	void onTextEnter( wxCommandEvent& event );
	void onKillFocus( wxFocusEvent& event );

	// UI controls.
	wxString X;
	wxString Y;
	wxString Z;

	wxTextCtrl* txt_X;
	wxTextCtrl* txt_Y;
	wxTextCtrl* txt_Z;

	wxChoice* choice_View;
	wxBitmapButton* btn_Wireframe;
	wxBitmapButton* btn_Textures;

private:

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces