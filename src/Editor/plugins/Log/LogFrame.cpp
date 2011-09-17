/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "LogFrame.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

LogFrame::LogFrame( wxWindow* parent )
	: wxListCtrl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES | wxLC_REPORT)
{
	wxListItem level;
	level.SetText("Level");
	InsertColumn(0, level);

	wxListItem message;
	message.SetText("Message");
	message.SetWidth(300);
	InsertColumn(1, message);

	Log* log = LogGetDefault();
	log->handlers.Connect( this, &LogFrame::Process );
}

//-----------------------------------//

LogFrame::~LogFrame()
{

}

//-----------------------------------//

void LogFrame::Process(LogEntry* entry)
{
	int id = InsertItem(GetItemCount(), wxEmptyString);
	//String level = LogLevel::toString( entry.level );

	SetItem(id, 0, wxEmptyString);
	SetItem(id, 1, entry->message);
}

//-----------------------------------//

NAMESPACE_EDITOR_END