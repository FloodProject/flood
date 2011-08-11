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
#include "Plugins/Scene/ScenePlugin.h"
#include "Plugins/Scene/ScenePane.h"
#include "Plugins/Scene/SceneDocument.h"
#include "Plugins/Property/PropertyPlugin.h"
#include "Plugins/Property/PropertyPage.h"
#include "Plugins/Networking/ServerPlugin.h"
#include "Network/Host.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ReplicaMessagesClient : ReplicaMessagePlugin
{
public:

	ReplicaMessagesClient();

	void sendObjectFieldUpdate(const FieldWatchVector& watches);
	void processReplicaInstance(const ReplicatedObject& rep);
	void handleReplicaNewInstance(const SessionPtr&, const ReplicaNewInstanceMessage&) OVERRIDE;
	void handleReplicaFullUpdate(const SessionPtr&, const ReplicaFullUpdateMessage&) OVERRIDE;
	void handleReplicaFieldUpdate(const SessionPtr&, const MessagePtr&) OVERRIDE;
};

REFLECT_CHILD_CLASS(ReplicaMessagesClient, ReplicaMessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

ReplicaMessagesClient::ReplicaMessagesClient()
{
	PropertyPlugin* pp = GetPlugin<PropertyPlugin>();
	pp->propertyPage->onClassFieldChanged.Bind(this, &ReplicaMessagesClient::sendObjectFieldUpdate);
}

//-----------------------------------//

void ReplicaMessagesClient::sendObjectFieldUpdate(const FieldWatchVector& watches)
{
	LogDebug("SendObjectFieldUpdate");

	// Encode fields and send update message.
	MemoryStream* ms = StreamCreateFromMemory(AllocatorGetThis(), 512);
	SerializerBinary* bin = (SerializerBinary*) SerializerCreateBinary(AllocatorGetThis());
	bin->ms = ms;

	ReflectionContext& context = bin->serializeContext;

	for(size_t i = 0; i < watches.size(); i++)
	{
		FieldWatch* fw = watches[i];
		context.field = fw->field;
		context.object = (Object*) fw->object;

		InstanceId instanceId = 0;
		if( !replicas.findInstance(fw->object, instanceId) )
		{
			LogDebug("Replicated instance of object not found");
			continue;
		}

		EncodeVariableInteger(ms, instanceId);
		ReflectionWalkCompositeField(&context);
	}

	// Finish field stream.
	EncodeVariableInteger(bin->ms, FieldInvalid);

	MessagePtr msg = MessageCreate(ReplicaMessageIds::ReplicaFieldUpdate);
	StreamWrite(msg->ms, ms->buf, ms->position);

	GetPlugin<ServerPlugin>()->host->broadcastMessage(msg);
}

//-----------------------------------//

void ReplicaMessagesClient::processReplicaInstance(const ReplicatedObject& rep)
{
	if( !rep.instance )
	{
		LogDebug("Expecting an object instance");
		return;
	}

	if( replicas.findInstanceById(rep.instanceId) )
	{
		LogDebug("Ignoring duplicated object replica");
		return;
	}

	registerObjects(rep.instance);

	Class* klass = ClassGetType(rep.instance);

	if( ClassInherits(klass, ReflectionGetType(Entity)) )
	{
		Entity* entity = (Entity*) rep.instance;
		
		ScenePlugin* sp = (ScenePlugin*) GetPlugin<ScenePlugin>();
		sp->scenePage->addEntity(entity);
	}
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaNewInstance(const SessionPtr& session, const ReplicaNewInstanceMessage& msg)
{
	ReplicatedObject obj;
	obj.instance = msg.instance;
	obj.instanceId = msg.instanceId;

	processReplicaInstance(obj);
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaFullUpdate(const SessionPtr&, const ReplicaFullUpdateMessage& msg)
{
	for(size_t i = 0; i < msg.objects.size(); i++)
	{
		const ReplicatedObject& obj = msg.objects[i];
		processReplicaInstance(obj);
	}
}

//-----------------------------------//

void ReplicaMessagesClient::handleReplicaFieldUpdate(const SessionPtr& session, const MessagePtr& msg)
{
	processFieldUpdate(msg);
}

//-----------------------------------//

NAMESPACE_EDITOR_END