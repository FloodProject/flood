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
};

REFLECT_CHILD_CLASS(ReplicaMessagesClient, ReplicaMessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaNewInstance(const SessionPtr& session, const ReplicaNewInstanceMessage& msg)
{
	if( !msg.instance )
	{
		LogDebug("ReplicaNewInstance: Expecting an object instance");
		return;
	}

	Class* klass = ClassGetType(msg.instance);

	if( ClassInherits(klass, ReflectionGetType(Entity)) )
	{
		Entity* entity = (Entity*) msg.instance;
		
		ScenePlugin* sp = (ScenePlugin*) GetPlugin<ScenePlugin>();
		sp->scenePage->addEntity(entity);
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END