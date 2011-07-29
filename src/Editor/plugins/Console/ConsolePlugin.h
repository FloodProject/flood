/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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