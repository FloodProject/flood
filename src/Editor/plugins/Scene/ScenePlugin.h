/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Protocol/ReplicaContext.h"
#include "Core/ClassWatcher.h"

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
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;
	void onEntitySelect( const EntityPtr& ) OVERRIDE;
	void onEntityUnselect( const EntityPtr& ) OVERRIDE;
	void onSceneLoad( const ScenePtr& scene ) OVERRIDE;
	void onSceneUnload( const ScenePtr& scene ) OVERRIDE;
	//void onDocumentCreate(Document& document) OVERRIDE;

	void onServerConnect(const SessionPtr&) OVERRIDE;
	void onSceneClassFieldUpdate(const FieldWatchVector&);
	void onReplicaContextCreate(ReplicaContext*, ClassId, ReplicaLocalId);
	void onReplicaObjectCreate(ReplicaContext*, ReplicaInstanceId, Object*);
	void onReplicaAdded(const ReplicatedObject&);
	ReplicaLocalId localId;

	ScenePage* scenePage;
	int iconScene;
};

//-----------------------------------//

NAMESPACE_EDITOR_END