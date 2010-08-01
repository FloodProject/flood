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

class ConsoleTextCtrl : public wxTextCtrl
{
public:

	ConsoleTextCtrl( vapor::Engine* engine, wxWindow* parent,
		wxWindowID id = wxID_ANY, const wxString& name = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,	
		const wxSize& size = wxDefaultSize, 
		long style = 0 | wxTE_MULTILINE | wxTE_NOHIDESEL | wxTE_RICH2 );

	virtual ~ConsoleTextCtrl() { }

protected:

	// Initializes the control.
	void InitControl();

	void OnEnter(/*wxCommandEvent& event*/);
	void OnKeyDown(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);

	void AppendTextColor( const wxString& text, const wxTextAttr& attr );

	void NewPromptLine();

	// Holds an instance of the input manager.
	State* luaState;

	// Holds an instance to the vaporEngine.
	vapor::Engine* engine;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces