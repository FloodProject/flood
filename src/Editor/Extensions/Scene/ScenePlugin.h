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

class wxFoldPanel;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ScenePage;
class PropertyPage;

typedef Event2<PropertyPage*, wxFoldPanel&> PropertyEvent;
typedef std::map<Class*, PropertyEvent> PropertyHandlerMap;

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

	// Sets the property handler for a class.
	void setPropertyHandler(Class*, const PropertyEvent&);

	// Removes the property handler for a class.
	void removePropertyHandler(Class*);

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

	// Shows the properties of the given entity.
	void showEntityProperties( Entity* entity );

	// Sets the bounding box visibility of the given entity.
	void setBoundingBoxVisible( const EntityPtr& entity, bool state );

#ifndef NO_NETWORK
	void onReplicaContextCreate(ReplicaContext*, ClassId, ReplicaLocalId);
	void onReplicaObjectCreate(ReplicaContext*, ReplicaInstanceId, Object*);
	void onReplicaAdded(const ReplicatedObject&);
	ReplicaLocalId localId;
#endif

	ScenePage* scenePage;
	int iconScene;

	PropertyHandlerMap propertyHandlers;
};

//-----------------------------------//

NAMESPACE_EDITOR_END