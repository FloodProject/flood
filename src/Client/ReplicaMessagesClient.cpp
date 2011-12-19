/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Protocol/ReplicaMessages.h"
#include "Core/ClassWatcher.h"
#include "Core/SerializationHelpers.h"
#include "Plugins/Networking/ServerPlugin.h"
#include "Network/Host.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ReplicaMessagesClient : ReplicaMessageHandler
{
public:

	ReplicaMessagesClient();

	void processReplicaInstance(ReplicaContextId contextId, const ReplicatedObject& rep);

	void handleReplicaContextCreated(const SessionPtr&, const ReplicaContextCreatedMessage&) OVERRIDE;
	void handleReplicaContextUpdate(const SessionPtr&, const ReplicaContextUpdateMessage&) OVERRIDE;

	void handleReplicaObjectCreated(const SessionPtr&, const ReplicaObjectCreatedMessage&) OVERRIDE;
	void handleReplicaObjectUpdate(const SessionPtr&, const MessagePtr&) OVERRIDE;
};

REFLECT_CHILD_CLASS(ReplicaMessagesClient, ReplicaMessageHandler)
REFLECT_CLASS_END()

//-----------------------------------//

ReplicaMessagesClient::ReplicaMessagesClient()
{

}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaContextCreated(const SessionPtr&, const ReplicaContextCreatedMessage& msg)
{
	LogDebug("ReplicaContextCreated: Received new replica context");

	ReplicaContext* context = AllocateThis(ReplicaContext);
	context->id = msg.contextId;

	replicaContexts[context->id] = context;

	onReplicaContextCreate(context, 0, msg.localId);
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaContextUpdate(const SessionPtr&, const ReplicaContextUpdateMessage& msg)
{
	for(size_t i = 0; i < msg.objects.size(); i++)
	{
		const ReplicatedObject& obj = msg.objects[i];
		processReplicaInstance(msg.contextId, obj);
	}
}

//-----------------------------------//

void ReplicaMessagesClient::processReplicaInstance(ReplicaContextId contextId, const ReplicatedObject& rep)
{
	if( !rep.instance )
	{
		LogDebug("Expecting an object instance");
		return;
	}

	ReplicaContext* context = findContext(contextId);

	if( !context )
	{
		LogDebug("processReplicaInstance: Invalid replica context");
		return;
	}

	if( context->findInstanceById(rep.instanceId) )
	{
		LogDebug("Ignoring duplicated object replica");
		return;
	}

	context->registerObjects(rep.instance);
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaObjectCreated(const SessionPtr& session, const ReplicaObjectCreatedMessage& msg)
{
	ReplicatedObject obj;
	obj.instance = msg.instance;
	obj.instanceId = msg.instanceId;

	processReplicaInstance(msg.contextId, obj);

	ReplicaContext* context = findContext(msg.contextId);

	if( !context )
	{
		LogDebug("ReplicaObjectCreated: Invalid replica context");
		return;
	}

	onReplicaObjectCreate(context, msg.parentId, msg.instance);
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaObjectUpdate(const SessionPtr& session, const MessagePtr& msg)
{
	ReplicaMessageHandler::handleReplicaObjectUpdate(session, msg);
}

//-----------------------------------//

NAMESPACE_EDITOR_END