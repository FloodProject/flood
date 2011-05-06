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

class LogFrame;

REFLECT_DECLARE_CLASS(LogPlugin)

class LogPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(LogPlugin)

public:

	LogPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Handles new button click.
	void onLogButtonClick(wxCommandEvent& event);

protected:

	// Log toolbar button.
	wxAuiToolBarItem* logButton;

	// Log frame.
	LogFrame* log;
};

//-----------------------------------//

} } // end namespaces