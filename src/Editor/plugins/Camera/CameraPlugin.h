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

class CameraControls;

//-----------------------------------//

class CameraPlugin : public Plugin
{
	REFLECT_DECLARE_CLASS()

public:

	CameraPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Scene load callback.
	virtual void onSceneLoad( const ScenePtr& scene );

	// Scene unload callback.
	virtual void onSceneUnload( const ScenePtr& scene );

	// Entity change callback.
	void onEntityChange(); 

protected:

	CameraControls* cameraControls;
};

//-----------------------------------//

} } // end namespaces