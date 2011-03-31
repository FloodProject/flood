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

/**
 * Shows the log stream in the editor.
 */

class LogFrame : public wxListCtrl
{
public:

	LogFrame( wxWindow* parent );
	void Process(LogEntry* entry);
};

//-----------------------------------//

} } // end namespaces