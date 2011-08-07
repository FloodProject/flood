/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Protocol/ReplicaMessages.h"
#include "Protocol/UserMessages.h"
#include "Protocol/Users.h"
#include "Network/Message.h"
#include "Network/Dispatcher.h"
#include "Network/Host.h"
#include "Server/Server.h"
#include "Core/Reflection.h"
#include "Engine/API.h"
#include "Scene/Scene.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

typedef std::map<uint64, Object*> ReplicasMap;

class ReplicaMessagesServer : ReplicaMessagePlugin
{
public:

	ReplicaMessagesServer();
	~ReplicaMessagesServer();

	void handleReplicaCreateInstance(const SessionPtr&, const ReplicaCreateInstanceMessage&) OVERRIDE;
	ReplicasMap replicas;
	Scene* scene;
};

REFLECT_CHILD_CLASS(ReplicaMessagesServer, ReplicaMessagePlugin)
REFLECT_CLASS_END()

ReplicaMessagesServer::ReplicaMessagesServer()
{
	scene = Allocate(Scene, AllocatorGetThis());
}

ReplicaMessagesServer::~ReplicaMessagesServer()
{
	Deallocate(scene);
}

//-----------------------------------//

void ReplicaMessagesServer::handleReplicaCreateInstance(const SessionPtr& session, const ReplicaCreateInstanceMessage& msg)
{
	Object* instance = msg.instance;
	Class* klass = ClassGetById(msg.classId);

	// Create a new instance if the client did not provide one.
	if( !instance )
	{
		if( !klass ) return;
		instance = (Object*) ClassCreateInstance(klass, AllocatorGetServer());
	}

	LogDebug("ReplicaCreateInstance: '%s'", klass->name);

	// Store the replicated object.
	uint64 instanceId = nextInstance++;
	replicas[instanceId] = instance;

	// Send the instance back to all the clients.
	ReplicaNewInstanceMessage inst;
	inst.instanceId = instanceId;
	inst.instance = instance;

	MessagePtr message = MessageCreate(ReplicaMessageIds::ReplicaNewInstance);
	message->write(&inst);

	if( ClassInherits(klass, ReflectionGetType(Entity)) )
	{
		Entity* entity = (Entity*) instance;
		scene->add(entity);
	}

	GetServer()->getHost()->broadcastMessage(message);
}

//-----------------------------------//

NAMESPACE_SERVER_END