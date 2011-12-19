/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Server/API.h"
#include "Server/ServerPlugin.h"
#include "Server/Server.h"
#include "Server/Plugins/ReplicaPlugin.h"
#include "Protocol/ReplicaMessages.h"
#include "Protocol/ReplicaContext.h"

class Scene;

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ScenePlugin)

class ScenePlugin : public ServerPlugin
{
	REFLECT_DECLARE_OBJECT(ScenePlugin)

public:

	ScenePlugin();

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	void onReplicaContextCreate(ReplicaContext*, ClassId, ReplicaLocalId);
	void onReplicaObjectCreate(ReplicaContext*, ReplicaInstanceId, Object*);
	void onReplicaAdded(const ReplicatedObject&);

	void referenceComponents();

	Scene* scene;
};

//-----------------------------------//

NAMESPACE_SERVER_END