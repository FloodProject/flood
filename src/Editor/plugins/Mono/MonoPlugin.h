/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

#ifdef ENABLE_PLUGIN_MONO

#include <mono/metadata/metadata.h>

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(MonoPlugin)

class MonoPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(MonoPlugin)

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

#endif
