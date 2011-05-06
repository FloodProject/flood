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

REFLECT_DECLARE_CLASS(SamplePlugin)

class SamplePlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(SamplePlugin)

public:

	SamplePlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();
};

//-----------------------------------//

} } // end namespaces