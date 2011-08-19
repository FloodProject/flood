/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/ServerPlugin.h"
#include "Server/Server.h"
#include "Protocol/ReplicaMessages.h"
#include "Protocol/ReplicaContext.h"

#include "Engine/API.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"

#include "Scene/Projector.h"
#include "Scene/Model.h"
#include "Scene/Grid.h"
#include "Scene/Light.h"
#include "Scene/Skydome.h"
#include "Scene/Camera.h"
#include "Scene/Billboard.h"
#include "Scene/Particles.h"
#include "Scene/Source.h"
#include "Scene/Listener.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class ScenePlugin : public ServerPlugin
{
public:

	ScenePlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	void onReplicaContextCreate(ReplicaContext*, ClassId, ReplicaLocalId);
	void onReplicaObjectCreate(ReplicaContext*, ReplicaInstanceId, Object*);
	void onReplicaAdded(const ReplicatedObject&);

	void referenceComponents();

	Scene* scene;
};

REFLECT_CHILD_CLASS(ScenePlugin, ServerPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

void ScenePlugin::referenceComponents()
{
	ProjectorGetType();
	GridGetType();
	ModelGetType();
	LightGetType();
	SkydomeGetType();
	CameraGetType();
	BillboardGetType();
	ParticlesGetType();
	SourceGetType();
	ListenerGetType();
}

ScenePlugin::ScenePlugin() : scene(nullptr)
{
	referenceComponents();
}

//-----------------------------------//

PluginMetadata ScenePlugin::getMetadata()
{
	PluginMetadata metadata;
	metadata.name = "Scene";
	metadata.description = "Provides scene services";
	return metadata;
}

//-----------------------------------//

void ScenePlugin::onPluginEnable()
{
	ReplicaMessagePlugin* rmp = GetMessagePlugin<ReplicaMessagePlugin>();
	rmp->onReplicaContextCreate.Connect(this, &ScenePlugin::onReplicaContextCreate);
	rmp->onReplicaObjectCreate.Connect(this, &ScenePlugin::onReplicaObjectCreate);
}

//-----------------------------------//

void ScenePlugin::onPluginDisable()
{
	Deallocate(scene);
}

//-----------------------------------//

void ScenePlugin::onReplicaContextCreate(ReplicaContext* context, ClassId classId, ReplicaLocalId)
{
	if( ReflectionGetType(Scene)->id == classId )
	{
		LogDebug("Creating scene");
		scene = Allocate(Scene, AllocatorGetThis());

		context->onReplicaAdded.Bind(this, &ScenePlugin::onReplicaAdded); 
	}
}

//-----------------------------------//

void ScenePlugin::onReplicaObjectCreate(ReplicaContext* context, ReplicaInstanceId parentId, Object* object)
{
	Class* klass = ClassGetType(object);

	if( ClassInherits(ComponentGetType(), klass) )
	{
		Component* component = (Component*) object;
	
		Entity* entity = (Entity*) context->findInstanceById(parentId);
		if( !entity ) return;

		entity->addComponent(component);
	}
	else if( ClassInherits(SceneGetType(), klass) )
	{
		Entity* entity = (Entity*) object;
		scene->add(entity);
	}
}

//-----------------------------------//

void ScenePlugin::onReplicaAdded(const ReplicatedObject& obj)
{
}

//-----------------------------------//

NAMESPACE_SERVER_END