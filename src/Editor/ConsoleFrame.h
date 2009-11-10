/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
// for all others, include the necessary headers (this file is usually all 
//  you need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/textctrl.h>

namespace vapor { 
	
	class Engine;
	
	namespace editor {

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