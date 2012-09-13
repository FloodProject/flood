/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ConsoleTextCtrl;

REFLECT_DECLARE_CLASS(ConsolePlugin)

class ConsolePlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(ConsolePlugin)

public:

	ConsolePlugin();

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

NAMESPACE_EDITOR_END