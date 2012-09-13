/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

class CameraControls;

//-----------------------------------//

REFLECT_DECLARE_CLASS(CameraPlugin)

class CameraPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(CameraPlugin)

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

NAMESPACE_EDITOR_END