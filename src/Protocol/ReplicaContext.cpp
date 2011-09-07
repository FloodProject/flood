/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Protocol/API.h"
#include "Protocol/ReplicaContext.h"
#include "Protocol/ReplicaMessages.h"
#include "Core/SerializationHelpers.h"
#include "Network/Session.h"
#include "Network/Host.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ReplicatedObject, Object)
	FIELD_PRIMITIVE(0, uint64, instanceId)
	FIELD_CLASS_PTR(2, Object, Object*, instance, RawPointer)
REFLECT_CLASS_END()

//-----------------------------------//

ReplicaContext::ReplicaContext()
	: nextInstance(0)
	, id(0)
{
}

//-----------------------------------//

void ReplicaContext::addReplica(const ReplicatedObject& obj)
{
	instances[obj.instance] = obj.instanceId;
	instancesIds[obj.instanceId] = obj;

	if( onReplicaAdded.empty() )
	{
		LogDebug("Replica context callback is empty");
		return;
	}

	onReplicaAdded(obj);
}

//-----------------------------------//

void ReplicaContext::removeReplica(const ReplicatedObject& obj)
{
	ReplicasMap::iterator it = instances.find(obj.instance);
	if( it != instances.end() ) instances.erase(it);

	ReplicasIdMap::iterator itIds = instancesIds.find(obj.instanceId);
	if( itIds != instancesIds.end() ) instancesIds.erase(itIds);
}

//-----------------------------------//

ReplicatedObject* ReplicaContext::findInstanceById(ReplicaInstanceId id)
{
	ReplicasIdMap::iterator it = instancesIds.find(id);
	
	if( it == instancesIds.end() )
		return nullptr;

	return &it->second;
}

//-----------------------------------//

bool ReplicaContext::findInstance(const Object* object, ReplicaInstanceId& id)
{
	ReplicasMap::iterator it = instances.find(object);
	
	if( it == instances.end() )
		return false;

	id = it->second;
	return true;
}

//-----------------------------------//

static void WalkComposite(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	ReplicaContext* rm = (ReplicaContext*) ctx->userData;

	if(wt == ReflectionWalkType::Begin)
	{
		ReplicatedObject rep;
		rep.instance = ctx->object;
		rep.instanceId = rm->nextInstance++;

		rm->addReplica(rep);
	}
}

void ReplicaContext::registerObjects(const Object* object)
{
	ReflectionContext context;
	context.userData = this;
	context.walkComposite = WalkComposite;

	ReflectionWalk((Object*) object, &context);
}

//-----------------------------------//

void ReplicaContext::processFieldUpdate(const MessagePtr& msg)
{
	uint64 val;
	DecodeVariableInteger(msg->ms, val);

	ReplicaInstanceId instanceId = val;
	ReplicatedObject* obj = findInstanceById(instanceId);

	if( !obj )
	{
		LogDebug("Instance id did not find replica object");
		return;
	}

	Allocator* alloc = AllocatorGetThis();
	SerializerBinary* bin = (SerializerBinary*) SerializerCreateBinary(alloc);
	bin->ms = msg->ms;

	ReflectionContext& dcontext = bin->deserializeContext;

	dcontext.object = obj->instance;
	dcontext.composite = ClassGetType(obj->instance);

	DeserializeFields(&dcontext);

	Deallocate(bin);
}

//-----------------------------------//

void ReplicaContext::sendObjects(const SessionPtr& session)
{
	ReplicaContextUpdateMessage full;

	ReplicasIdMap::iterator it;
	
	for(it = instancesIds.begin(); it != instancesIds.end(); ++it)
	{
		ReplicatedObject& obj = it->second;
		full.objects.push_back(obj);
	}

	MessagePtr message = MessageCreate(ReplicaMessageIds::ReplicaContextUpdate);
	message->write(&full);

	session->getPeer()->queueMessage(message, 0);
}

//-----------------------------------//

MessagePtr ReplicaContext::createObjectUpdateMessage(const FieldWatchVector& watches)
{
	// Encode fields and send update message.
	MemoryStream* ms = StreamCreateFromMemory(AllocatorGetThis(), 512);
	SerializerBinary* bin = (SerializerBinary*) SerializerCreateBinary(AllocatorGetThis());
	bin->ms = ms;

	EncodeVariableInteger(bin->ms, id);

	ReflectionContext& refContext = bin->serializeContext;

	for(size_t i = 0; i < watches.size(); i++)
	{
		FieldWatch* fw = watches[i];

		if( !FieldIsSerializable(fw->field) )
			continue;

		refContext.field = fw->field;
		refContext.object = (Object*) fw->object;

		ReplicaInstanceId instanceId = 0;
		if( !findInstance((Object*) fw->object, instanceId) )
		{
			LogDebug("Replicated instance of object not found");
			continue;
		}

		EncodeVariableInteger(ms, instanceId);
		ReflectionWalkCompositeField(&refContext);
	}

	// Finish field stream.
	EncodeVariableInteger(bin->ms, FieldInvalid);

	MessagePtr msg = MessageCreate(ReplicaMessageIds::ReplicaObjectUpdate);
	StreamWrite(msg->ms, ms->buffer, ms->position);

	return msg;
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END