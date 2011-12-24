/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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