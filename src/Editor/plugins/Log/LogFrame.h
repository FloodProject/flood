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

/**
 * Shows the log stream in the editor.
 */

class LogFrame : public wxFrame
{
public:

	LogFrame( wxWindow* parent, const wxString&	name = "Log" );

protected:

	// Initializes the control.
	void InitControl();

	// Event handlers.
	void OnClose(wxCloseEvent& event);

	// Text control to enter commands.
	wxPanel* panel;
	wxListCtrl* listCtrl;

};

//-----------------------------------//

} } // end namespaces