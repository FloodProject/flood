/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ConsoleFrame.h"
#include "ConsoleTextCtrl.h"

namespace vapor { namespace editor {

//-----------------------------------//

ConsoleFrame::ConsoleFrame( Engine* engine, wxWindow* parent, 
						   const wxString& name )
	: wxFrame( parent, wxID_ANY, name, wxDefaultPosition,
	wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxFRAME_TOOL_WINDOW |
	wxFRAME_FLOAT_ON_PARENT | wxBORDER_NONE )
	, engine( engine )
{
	InitControl();
}

//-----------------------------------//

void ConsoleFrame::InitControl()
{
	textCtrl = new ConsoleTextCtrl( engine, this );
	Bind( wxEVT_CLOSE_WINDOW, &ConsoleFrame::OnClose, this );
}

//-----------------------------------//

void ConsoleFrame::OnClose(wxCloseEvent& event)
{
    if ( event.CanVeto() )
    {
		// Hide the window instead of closing it.
		Hide();

        event.Veto();
        return;   
    }

    event.Skip();
}

//-----------------------------------//

} } // end namespaces