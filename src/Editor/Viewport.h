/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
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

	Viewport( vapor::Engine* engine, wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL );

	~Viewport();
		
	scene::NodePtr cameraNode;
	scene::CameraPtr camera;
	scene::TransformPtr transform;

	// vaporEngine's wxWidgets control.
	vaporControl* vaporCtrl;

protected:

	void build();
	void init();
	void createCamera();
	void updatePosition();

	// Event handlers
	void OnToolChoice( wxCommandEvent& event );
	void onCameraTransform();
	void onText( wxCommandEvent& event );
	void onTextEnter( wxCommandEvent& event );
	void onKillFocus( wxFocusEvent& event );

	wxString X;
	wxString Y;
	wxString Z;

	wxStaticText* lbl_X;
	wxTextCtrl* txt_X;
	wxStaticText* lbl_Y;
	wxTextCtrl* txt_Y;
	wxStaticText* lbl_Z;
	wxTextCtrl* txt_Z;
	wxStaticLine* m_staticline2;
	wxChoice* choice_View;
	wxBitmapButton* btn_Wireframe;
	wxBitmapButton* btn_Textures;

	vapor::Engine* engine;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces