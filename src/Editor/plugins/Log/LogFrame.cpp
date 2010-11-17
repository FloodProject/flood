/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "LogFrame.h"

namespace vapor { namespace editor {

//-----------------------------------//

class LogSinkFrame : public LogSink
{
public:
	LogSinkFrame( wxListCtrl* listCtrl ) 
		: listCtrl(listCtrl)
	{}
	
	void process(LogEntry& entry)
	{
		int id = listCtrl->InsertItem(listCtrl->GetItemCount(), wxEmptyString);
		std::string level = LogLevel::toString( entry.level );

		listCtrl->SetItem(id, 0, level);
		listCtrl->SetItem(id, 1, entry.message);

		if(entry.level == LogLevel::Warning)
			listCtrl->SetItemBackgroundColour(id, wxColour(236, 242, 69));
	}
	
	wxListCtrl* listCtrl;
};

//-----------------------------------//

LogFrame::LogFrame( Engine* engine, wxWindow* parent, const wxString& name )
	: wxFrame( parent, wxID_ANY, name, wxDefaultPosition,
	wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxFRAME_TOOL_WINDOW |
	wxFRAME_FLOAT_ON_PARENT | wxBORDER_NONE )
	, engine(engine)
{
	InitControl();

	wxListItem level;
	level.SetText("Level");
	listCtrl->InsertColumn(0, level);

	wxListItem message;
	message.SetText("Message");
	message.SetWidth(1000);
	listCtrl->InsertColumn(1, message);

	Logger* log = engine->getLogger();
	log->add( new LogSinkFrame(listCtrl) );
}

//-----------------------------------//

void LogFrame::InitControl()
{
	//SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	panel = new wxPanel( this, wxID_ANY );
	
	wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
	mainSizer->Add( panel, 1, wxEXPAND, 5 );

	listCtrl = new wxListCtrl( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT );
	
	wxBoxSizer* panelSizer = new wxBoxSizer( wxVERTICAL );
	panelSizer->Add( listCtrl, 1, wxEXPAND, 5 );
	
	panel->SetSizerAndFit( panelSizer );
	SetSizerAndFit( mainSizer );

	Bind( wxEVT_CLOSE_WINDOW, &LogFrame::OnClose, this );
}

//-----------------------------------//

void LogFrame::OnClose(wxCloseEvent& event)
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