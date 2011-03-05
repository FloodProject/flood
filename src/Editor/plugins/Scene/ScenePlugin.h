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

class ScenePage;

class ScenePlugin : public Plugin
{
public:
 
	ScenePlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Entity selection callback.
	virtual void onEntitySelect( const EntityPtr& );
	
	// Entity unselection callback.
	virtual void onEntityUnselect( const EntityPtr& );

	// Scene load callback.
	virtual void onSceneLoad( const ScenePtr& scene );

	ScenePage* scenePage;
	int iconScene;
};

//-----------------------------------//

} } // end namespaces