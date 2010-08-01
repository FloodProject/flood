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
 * Text console used to interact with the rest of the engine code via
 * scripting (for now it hooks up with the Lua VM subystem).
 */

class ConsoleFrame : public wxFrame 
{
public:

	ConsoleFrame( vapor::Engine* engine, 
					wxWindow* parent, 
					const wxString&	name = "ConsoleFrame" );

	virtual ~ConsoleFrame();

protected:

	// Initializes the control.
	void InitControl();

	// Event handlers.
	void OnClose(wxCloseEvent& event);

	// Holds an instance to the vaporEngine.
	vapor::Engine* engine;

	// Text control to enter commands.
	wxTextCtrl *textCtrl;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces