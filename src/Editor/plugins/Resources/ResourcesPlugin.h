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

class ResourcesPage;
class ResourcesBrowser;

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourcesPlugin)

class ResourcesPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(ResourcesPlugin)

public:

	ResourcesPlugin();
	~ResourcesPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Handles button click.
	void onBrowserButtonClick(wxCommandEvent& event);

protected:

	ResourcesPage* resourcesPage;
	ResourcesBrowser* resourcesBrowser;
	wxAuiToolBarItem* resourcesBrowserButton;
	int iconResources;
};

//-----------------------------------//

} } // end namespaces