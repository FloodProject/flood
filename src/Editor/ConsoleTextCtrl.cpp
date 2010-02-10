/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
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

#define TEXT_COLOR wxColour( "LIGHT GREY" )
#define ERROR_COLOR wxColour( "#C80000" )

void ConsoleTextCtrl::InitControl()
{
	SetBackgroundColour( *wxBLACK );
	SetForegroundColour( TEXT_COLOR );

	Bind( wxEVT_SET_FOCUS, &ConsoleTextCtrl::OnSetFocus, this ); 

	// The way these operations are ordered might seem stupid but
	// any other order didn't seem to work... might be a wx bug.

	AppendTextColor( 
		wxString::Format("Welcome to vaporEditor Console [%s]\n\n", VAPOR_ENGINE_VERSION ),
		*wxWHITE ); 

	AppendTextColor( ">", *wxWHITE );

	wxTextAttr style( TEXT_COLOR );
	SetDefaultStyle( style );

	AppendText( " " );

	luaState = engine->getScriptState();
}

//-----------------------------------//

void ConsoleTextCtrl::NewPromptLine()
{
	AppendTextColor( "\n> ", *wxWHITE );
}

//-----------------------------------//

void ConsoleTextCtrl::AppendTextColor( const wxString& text, const wxTextAttr& attr )
{
	const wxTextAttr oldStyle = GetDefaultStyle();

	SetDefaultStyle( attr );
	AppendText( text );
	SetDefaultStyle( oldStyle );
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

	if( event.ControlDown() && key == 'L' )
	{
		// Ctrl-L can be used to clear the console
		Clear();
		NewPromptLine();
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

	// NOTE:
	// It seems PositionToXY isn't implemented in OSX, so beware when/if
	// this someday gets ported to the Mac.

	long x, y;
	PositionToXY( GetLastPosition(), &x, &y );

	//wxString txt = GetRange( XYToPosition( 2, y ), GetLastPosition() );

	std::string text( GetRange( XYToPosition( 2, y ), GetLastPosition() ).c_str() );

#ifdef VAPOR_SCRIPTING_LUA
	if( !luaState->execute( text.c_str() ) )
	{
		AppendText( "\n" );
		AppendTextColor( luaState->getLastError(), ERROR_COLOR );
	}
#else
	{
		AppendText( "\n" );
		AppendTextColor( "Lua scripting support is disabled", ERROR_COLOR );
	}
#endif

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