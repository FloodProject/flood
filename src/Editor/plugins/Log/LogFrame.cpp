/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "LogFrame.h"
#include "Editor.h"

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
	}
	
	wxListCtrl* listCtrl;
};

//-----------------------------------//

LogFrame::LogFrame( wxWindow* parent )
	: wxListCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES | wxLC_REPORT)
{
	wxListItem level;
	level.SetText("Level");
	InsertColumn(0, level);

	wxListItem message;
	message.SetText("Message");
	message.SetWidth(1000);
	InsertColumn(1, message);

	Logger* log = GetEditor().getEngine()->getLogger();
	log->add( new LogSinkFrame(this) );
}

//-----------------------------------//

} } // end namespaces