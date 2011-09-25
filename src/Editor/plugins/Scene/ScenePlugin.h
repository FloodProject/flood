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

	// Scene callbacks.
	void onSceneLoad( const ScenePtr& ) OVERRIDE;
	void onSceneUnload( const ScenePtr& ) OVERRIDE;
	void onServerConnect(const SessionPtr&) OVERRIDE;

	// Entity callbacks.
	void onEntitySelect( const EntityPtr& ) OVERRIDE;
	void onEntityUnselect( const EntityPtr& ) OVERRIDE;

	// Component callbacks.
	void onComponentSelect( const ComponentPtr& ) OVERRIDE;
	void onComponentUnselect( const ComponentPtr& ) OVERRIDE;
	void onComponentChanged(const ComponentPtr& );
	
	// Widget handlers.
	void onPlayCommand(wxCommandEvent&);
	void onSceneClassFieldUpdate(const FieldWatchVector&);

	void showEntityProperties( Entity* entity );

#ifndef NO_NETWORK
	void onReplicaContextCreate(ReplicaContext*, ClassId, ReplicaLocalId);
	void onReplicaObjectCreate(ReplicaContext*, ReplicaInstanceId, Object*);
	void onReplicaAdded(const ReplicatedObject&);
	ReplicaLocalId localId;
#endif

	ScenePage* scenePage;
	int iconScene;
};

//-----------------------------------//

NAMESPACE_EDITOR_END