/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

class ConsoleTextCtrl;

class ConsolePlugin : public Plugin
{
public:

	ConsolePlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Handles new button click.
	void onConsoleButtonClick(wxCommandEvent& event);

protected:

	// Console toolbar button.
	wxAuiToolBarItem* consoleButton;

	// Console.
	ConsoleTextCtrl* console;
};

//-----------------------------------//

} } // end namespaces