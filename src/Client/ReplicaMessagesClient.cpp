/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Protocol/ReplicaMessages.h"
#include "Editor.h"
#include "plugins/Scene/ScenePlugin.h"
#include "plugins/Scene/ScenePane.h"
#include "plugins/Scene/SceneDocument.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ReplicaMessagesClient : ReplicaMessagePlugin
{
	void handleReplicaNewInstance(const SessionPtr&, const ReplicaNewInstanceMessage&) OVERRIDE;
	void handleReplicaFullUpdate(const SessionPtr&, const ReplicaFullUpdateMessage&) OVERRIDE;
};

REFLECT_CHILD_CLASS(ReplicaMessagesClient, ReplicaMessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

static void ProcessReplicaInstance(Object* instance)
{
	if( !instance )
	{
		LogDebug("Expecting an object instance");
		return;
	}

	Class* klass = ClassGetType(instance);

	if( ClassInherits(klass, ReflectionGetType(Entity)) )
	{
		Entity* entity = (Entity*) instance;
		
		ScenePlugin* sp = (ScenePlugin*) GetPlugin<ScenePlugin>();
		sp->scenePage->addEntity(entity);
	}
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaNewInstance(const SessionPtr& session, const ReplicaNewInstanceMessage& msg)
{
	ProcessReplicaInstance(msg.instance);
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaFullUpdate(const SessionPtr&, const ReplicaFullUpdateMessage& msg)
{
	for(size_t i = 0; i < msg.objects.size(); i++)
	{
		const ReplicatedObject& obj = msg.objects[i];
		ProcessReplicaInstance(obj.instance);
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END