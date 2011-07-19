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

class ScenePage;

REFLECT_DECLARE_CLASS(ScenePlugin)

class ScenePlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(ScenePlugin)

public:
 
	ScenePlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	virtual void onPluginEnable() OVERRIDE;
	virtual void onPluginDisable() OVERRIDE;

	// Entity callbacks.
	virtual void onEntitySelect( const EntityPtr& ) OVERRIDE;
	virtual void onEntityUnselect( const EntityPtr& ) OVERRIDE;

	// Scene callbacks.
	virtual void onSceneLoad( const ScenePtr& scene ) OVERRIDE;
	virtual void onSceneUnload( const ScenePtr& scene ) OVERRIDE;

	ScenePage* scenePage;
	int iconScene;
};

//-----------------------------------//

NAMESPACE_EDITOR_END