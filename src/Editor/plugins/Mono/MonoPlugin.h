/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

#include <mono/metadata/metadata.h>

namespace vapor { namespace editor {

//-----------------------------------//

class MonoPlugin : public Plugin
{
	REFLECT_DECLARE_CLASS()

public:

	MonoPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	MonoDomain* domain;
	MonoDomain* domainEditor;
};

//-----------------------------------//

} } // end namespaces