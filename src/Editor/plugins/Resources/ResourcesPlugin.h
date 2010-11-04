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

class ResourcesPlugin : public Plugin
{
public:

	ResourcesPlugin( EditorFrame* frame );
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
	wxToolBarToolBase* resourcesBrowserButton;
	int iconResources;
};

//-----------------------------------//

} } // end namespaces