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

class HierarchyPane;

class HierarchyPlugin : public Plugin
{
	REFLECT_DECLARE_CLASS()

public:

	HierarchyPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

protected:

	HierarchyPane* hierarchyPane;
};

//-----------------------------------//

} } // end namespaces