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

#include <vapor/Engine.h>
#include <vapor/script/State.h>

namespace vapor { namespace editor {

//-----------------------------------//

class ConsoleTextCtrl : public wxTextCtrl
{
public:

	ConsoleTextCtrl( vapor::Engine* engine, wxWindow* parent,
		wxWindowID id = wxID_ANY, const wxString& name = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,	
		const wxSize& size = wxDefaultSize, 
		long style = 0 | wxTE_MULTILINE | wxTE_NOHIDESEL | wxTE_RICH2 );

	virtual ~ConsoleTextCtrl();

protected:

	// Initializes the control.
	void InitControl();

	void OnEnter(/*wxCommandEvent& event*/);
	void OnKeyDown(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);

	void AppendTextColor( const wxString& text, const wxTextAttr& attr );

	void NewPromptLine();

	// Holds an instance of the input manager.
	script::State* luaState;

	// Holds an instance to the vaporEngine.
	vapor::Engine* engine;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces