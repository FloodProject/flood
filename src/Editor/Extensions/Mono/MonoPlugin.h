/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

#ifdef ENABLE_PLUGIN_MONO

#include <mono/metadata/metadata.h>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(MonoPlugin)

class MonoPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(MonoPlugin)

public:

	MonoPlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin enable callback.
	void onPluginRegistered() OVERRIDE;
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	MonoDomain* domain;
	MonoDomain* domainEditor;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif
