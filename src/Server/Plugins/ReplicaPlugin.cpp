/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Plugins/ReplicaPlugin.h"
#include "Protocol/UserMessages.h"
#include "Protocol/Users.h"
#include "Server/Server.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ReplicaPlugin, ServerPlugin)
REFLECT_CLASS_END()

PROTOCOL_PLUGIN_BEGIN(ReplicaPlugin)
	METADATA_NAME(Replica)
	METADATA_DESC(Provides object replication functionality.)
	METADATA_AUTHOR(triton)
	METADATA_VERSION(1.0)
	METADATA_PRIORITY(20)
PROTOCOL_PLUGIN_END()

//-----------------------------------//

void ReplicaPlugin::handleReplicaContextCreate(const SessionPtr& session, const ReplicaContextCreateMessage& msg)
{
	LogDebug("ReplicaContextCreate: %s", "Creating a new context");

	// Create a new replica context.
	ReplicaContext* context = AllocateThis(ReplicaContext);
	context->id = nextContextId++;
	replicaContexts[context->id] = context;

	onReplicaContextCreate(context, msg.classId, msg.localId);

	// Broadcast the new created context.
	ReplicaContextCreatedMessage created;
	created.contextId = context->id;
	created.localId = msg.localId;

	MessagePtr m_created = MessageCreate(ReplicaMessageIds::ReplicaContextCreated);
	m_created->write(&created);

	GetServer()->getHost()->broadcastMessage(m_created);
}

//-----------------------------------//

void ReplicaPlugin::handleReplicaContextRequest(const SessionPtr& session, const ReplicaContextRequestMessage& ask)
{
	ReplicaContext* context = findContext(ask.contextId);

	if( !context )
	{
		LogDebug("ReplicaContextRequest: Invalid replica context");
		return;
	}

	context->sendObjects(session);
}

//-----------------------------------//

void ReplicaPlugin::handleReplicaObjectCreate(const SessionPtr& session, const ReplicaObjectCreateMessage& msg)
{
	ReplicaContext* context = findContext(msg.contextId);

	if( !context )
	{
		LogDebug("ReplicaObjectCreate: Invalid replica context");
		return;
	}

	Object* instance = msg.instance;
	Class* klass = ClassGetById(msg.classId);

	if( !instance && !klass )
	{
		LogDebug("ReplicaObjectCreate: Invalid instance or class id");
		return;
	}

	// Create a new instance if the client did not provide one.
	if( !instance )
		instance = (Object*) ClassCreateInstance(klass, AllocatorGetServer());

	if( !klass )
		klass = ClassGetType(instance);

	// Register the replicated object instance.
	LogDebug("ReplicaObjectCreate: Registering instance of '%s'", klass->name);
	context->registerObjects(instance);

	onReplicaObjectCreate(context, msg.parentId, instance);

	// Send the instance back to all the clients.
	ReplicaInstanceId id;
	context->findInstance(instance, id);

	ReplicaObjectCreatedMessage inst;
	inst.contextId = context->id;
	inst.parentId = msg.parentId;
	inst.localId = msg.localId;
	inst.instanceId = id;
	inst.instance = instance;

	MessagePtr m_inst = MessageCreate(ReplicaMessageIds::ReplicaObjectCreated);
	m_inst->write(&inst);

	GetServer()->getHost()->broadcastMessage(m_inst);
}

//-----------------------------------//

void ReplicaPlugin::handleReplicaObjectUpdate(const SessionPtr& session, const MessagePtr& msg)
{
	ReplicaMessageHandler::handleReplicaObjectUpdate(session, msg);

	MessagePtr m_update = MessageCreate(ReplicaMessageIds::ReplicaObjectUpdate);
	StreamWrite(m_update->ms, msg->ms->buffer, msg->ms->position);
	GetServer()->getHost()->broadcastMessage(m_update);
}

//-----------------------------------//

NAMESPACE_SERVER_END