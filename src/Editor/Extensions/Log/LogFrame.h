/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

/**
 * Shows the log stream in the editor.
 */

class LogFrame : public wxListCtrl
{
public:

	LogFrame( wxWindow* parent );
	~LogFrame();

	// Event callbacks.
	void Process(LogEntry* entry);
	void OnIdle(wxIdleEvent& event);

	ConcurrentQueue<LogEntry> entries;
};

//-----------------------------------//

NAMESPACE_EDITOR_END