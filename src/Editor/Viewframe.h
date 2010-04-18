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

class Viewframe : public wxPanel
{
public:

	Viewframe( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL );

	// Creates and adds a new viewport to the viewframe.
	Viewport* createViewport( NodePtr node );

	// Gets the associated camera.
	IMPLEMENT_GETTER(Camera, const CameraPtr, viewport->getCamera())

	// Gets the associated control.
	IMPLEMENT_GETTER(Control, vaporControl*, control)

	// Gets the associated viewport.
	IMPLEMENT_GETTER(Viewport, ViewportPtr, viewport)

protected:

	void build();
	void updatePosition();

	// Event handlers.
	void OnToolChoice( wxCommandEvent& event );
	void onCameraTransform();
	void onText( wxCommandEvent& event );
	void onTextEnter( wxCommandEvent& event );
	
	// Control where the scene will be drawn.
	vaporControl* control;

	// Viewport associated with the control.
	ViewportPtr viewport;

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