/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ConsoleTextCtrl.h"
#include "lua.hpp"

namespace vapor { namespace editor {

//-----------------------------------//

#define TEXT_COLOR wxColour("LIGHT GREY")
#define EVAL_COLOR *wxWHITE
#define ERROR_COLOR wxColour("#C80000")

static ConsoleTextCtrl* console = nullptr;

//-----------------------------------//

#ifdef VAPOR_SCRIPTING_LUA

int luaPrintStub(lua_State* L)
{
	if( !console )
		return 1;

	return console->luaPrint(L);
}

#endif

//-----------------------------------//

ConsoleTextCtrl::ConsoleTextCtrl( vapor::Engine* engine, wxWindow* parent,
		wxWindowID id, const wxString& name, const wxPoint& pos, 
		const wxSize& size, long style )
		: wxTextCtrl( parent, id, name, pos, size, style )
		, engine( engine )
{
	console = this;
	InitControl();

#ifdef VAPOR_SCRIPTING_LUA
	luaState = engine->getScriptManager()->getState();
	lua_register( luaState->getLuaState() , "print", luaPrintStub );
#endif
}

//-----------------------------------//

ConsoleTextCtrl::~ConsoleTextCtrl()
{
	console = nullptr;
}

//-----------------------------------//

void ConsoleTextCtrl::InitControl()
{
	Bind( wxEVT_SET_FOCUS, &ConsoleTextCtrl::OnSetFocus, this );
	Bind( wxEVT_KEY_DOWN, &ConsoleTextCtrl::OnKeyDown, this );

	SetBackgroundColour( *wxBLACK );

	wxTextAttr attr;
	attr.SetBackgroundColour( *wxBLACK );
	attr.SetTextColour( TEXT_COLOR );
	SetDefaultStyle(attr);

	// The way these operations are ordered might seem stupid but
	// any other order didn't seem to work... might be a wx bug.

	AppendTextColor( wxString::Format(
		"Welcome to " VAPOR_EDITOR_NAME " Console [%s]\n\n", VAPOR_ENGINE_VERSION ),
		*wxWHITE ); 

	AppendTextColor( ">", EVAL_COLOR );
	AppendText( " " );
}

//-----------------------------------//

#ifdef VAPOR_SCRIPTING_LUA

int ConsoleTextCtrl::luaPrint(lua_State* L)
{
	int numArgs = lua_gettop(L);
	lua_getglobal(L, "tostring");
	
	std::string ret;
	ret.append("\n");
	
	for(int i = 1; i <= numArgs; i++)
	{
		const char *s;
		
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		
		s = lua_tostring(L, -1);
		
		if( !s )
		{
			return luaL_error(L, LUA_QL("tostring") " must return a string to ",
			LUA_QL("print"));
		}

		if(i > 1)
			ret.append("\t");

		ret.append(s);
		lua_pop(L, 1);
	}

	AppendTextColor( ret, EVAL_COLOR );

	return 0;
}

#endif

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

	if( key == WXK_RETURN || key == WXK_NUMPAD_ENTER )
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