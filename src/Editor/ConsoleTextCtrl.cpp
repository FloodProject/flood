/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "ConsoleTextCtrl.h"

namespace vapor { namespace editor {

//-----------------------------------//

using namespace vapor::script;

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ConsoleTextCtrl, wxTextCtrl)
	//EVT_SET_FOCUS( ConsoleTextCtrl::OnSetFocus )
	EVT_KEY_DOWN( ConsoleTextCtrl::OnKeyDown )
	//EVT_TEXT_ENTER( wxID_ANY, ConsoleTextCtrl::OnEnter )
END_EVENT_TABLE()

//-----------------------------------//

ConsoleTextCtrl::ConsoleTextCtrl( vapor::Engine* engine, wxWindow* parent,
		wxWindowID id, const wxString& name, const wxPoint& pos, 
		const wxSize& size, long style )
		: wxTextCtrl( parent, id, name, pos, size, style ), engine( engine )
{
	InitControl();
}

//-----------------------------------//

ConsoleTextCtrl::~ConsoleTextCtrl()
{

}

//-----------------------------------//

void ConsoleTextCtrl::InitControl()
{
	SetBackgroundColour( *wxBLACK );
	//SetForegroundColour( *wxWHITE );

	wxTextAttr style( wxColour( "LIGHT GREY" ) );
	SetDefaultStyle( style );

	Bind( wxEVT_SET_FOCUS, &ConsoleTextCtrl::OnSetFocus, this ); 

	//Clear();

	/*ChangeValue( "> " ); */
	AppendText( "> " ); 

	luaState = engine->getScriptState();
}

//-----------------------------------//

void ConsoleTextCtrl::NewPromptLine()
{
	AppendText( "\n> " );
	//SetStyle( 
}

//-----------------------------------//

void ConsoleTextCtrl::OnKeyDown(wxKeyEvent& event)
{
	int key = event.GetKeyCode();

	// Disallows overwriting the command prompt.
	if( key == WXK_BACK || key == WXK_DELETE )
	{
		int cur = GetInsertionPoint();
		
		long x, y;
		PositionToXY( cur, &x, &y );

		if( GetStringSelection().empty() )
		{
			if( x <= 2 && key == WXK_BACK )
				return;

			if( x >= 0 && x <= 1 && key == WXK_DELETE )
				return;
		}
		else
		{
			long from, to;
			GetSelection( &from, &to );
			
			if( from <= 1 ) return;		
		}
	}

	if( key == WXK_RETURN )
	{
		OnEnter();
		return;
	}

	event.Skip();
}

//-----------------------------------//

void ConsoleTextCtrl::OnEnter(/*wxCommandEvent& event*/)
{
	// Remove extra \n inserted by pressing Return
	//Remove( GetLastPosition(), GetLastPosition()-1 );
	//SetInsertionPointEnd();

	// It seems PositionToXY isn't implemented in OSX, so beware when/if
	// this someday gets ported to the Mac.

	long x, y;
	PositionToXY( GetLastPosition(), &x, &y );

	//wxString txt = GetRange( XYToPosition( 2, y ), GetLastPosition() );

	std::string text( GetRange( XYToPosition( 2, y ), GetLastPosition() ).c_str() );

	if( !luaState->execute( text.c_str() ) )
	{
		AppendText( "\n" );
		AppendText( luaState->getLastError() );
	}

	NewPromptLine();
}

//-----------------------------------//

void ConsoleTextCtrl::OnSetFocus(wxFocusEvent& event)
{
	SetInsertionPointEnd();
	event.Skip();
	Unbind( wxEVT_SET_FOCUS, &ConsoleTextCtrl::OnSetFocus, this );
}


//-----------------------------------//

} } // end namespaces