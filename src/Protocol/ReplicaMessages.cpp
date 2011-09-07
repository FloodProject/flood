/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Protocol/API.h"
#include "Protocol/ReplicaMessages.h"
#include "Network/MessageHandlers.h"
#include "Core/Object.h"
#include "Core/SerializationHelpers.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ReplicaContextCreateMessage, MessageDefinition)
	FIELD_PRIMITIVE(1, uint16, localId)
	FIELD_PRIMITIVE(2, uint16, classId)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaContextCreatedMessage, MessageDefinition)
	FIELD_PRIMITIVE(1, uint8, contextId)
	FIELD_PRIMITIVE(2, uint16, localId)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaContextUpdateMessage, MessageDefinition)
	FIELD_PRIMITIVE(1, uint8, contextId)
	FIELD_VECTOR(2, ReplicatedObject, objects)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaContextRequestMessage, MessageDefinition)
	FIELD_PRIMITIVE(3, uint8, contextId)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaObjectCreateMessage, MessageDefinition)
	FIELD_PRIMITIVE(1, uint8, contextId)
	FIELD_PRIMITIVE(2, uint16, localId)
	FIELD_PRIMITIVE(3, uint16, classId)
	FIELD_PRIMITIVE(4, uint16, parentId)
	FIELD_CLASS_PTR(5, Object, Object*, instance, RawPointer)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaObjectCreatedMessage, MessageDefinition)
	FIELD_PRIMITIVE(1, uint8, contextId)
	FIELD_PRIMITIVE(2, uint16, localId)
	FIELD_PRIMITIVE(3, uint16, instanceId)
	FIELD_PRIMITIVE(4, uint16, parentId)
	FIELD_CLASS_PTR(5, Object, Object*, instance, RawPointer)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaObjectDeleteMessage, MessageDefinition)
	FIELD_PRIMITIVE(1, uint8, contextId)
	FIELD_PRIMITIVE(2, uint16, instanceId)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaObjectUpdateMessage, MessageDefinition)
REFLECT_CLASS_END()

REFLECT_ENUM(ReplicaMessageIds)
	ENUM(ReplicaContextCreate)
	ENUM(ReplicaContextCreated)
	ENUM(ReplicaContextRequest)
	ENUM(ReplicaContextUpdate)
	
	ENUM(ReplicaObjectCreate)
	ENUM(ReplicaObjectCreated)
	ENUM(ReplicaObjectDelete)
	ENUM(ReplicaObjectUpdate)
REFLECT_ENUM_END()

IMPLEMENT_HANDLER_REF(Replica, ReplicaContextCreate)
IMPLEMENT_HANDLER_REF(Replica, ReplicaContextCreated)
IMPLEMENT_HANDLER_REF(Replica, ReplicaContextRequest)
IMPLEMENT_HANDLER_REF(Replica, ReplicaContextUpdate)

IMPLEMENT_HANDLER_REF(Replica, ReplicaObjectCreate)
IMPLEMENT_HANDLER_REF(Replica, ReplicaObjectDelete)
IMPLEMENT_HANDLER_REF(Replica, ReplicaObjectCreated)

PROTOCOL_MESSAGE_HANDLERS(Replica)
	HANDLER_REF(Replica, ReplicaContextCreate, ClientToServer)
	HANDLER_REF(Replica, ReplicaContextCreated, ServerToClient)
	HANDLER_REF(Replica, ReplicaContextRequest, ClientToServer)
	HANDLER_REF(Replica, ReplicaContextUpdate, ServerToClient)

	HANDLER_REF(Replica, ReplicaObjectCreate, ClientToServer)
	HANDLER_REF(Replica, ReplicaObjectCreated, ServerToClient)
	HANDLER_REF(Replica, ReplicaObjectDelete, Both)
	HANDLER_RAW(Replica, ReplicaObjectUpdate, Both)
PROTOCOL_MESSAGE_HANDLERS_END()

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ReplicaMessageHandler, MessageHandler)
REFLECT_CLASS_END()

//-----------------------------------//

const MessagesTable& ReplicaMessageHandler::getMessagesTable()
{
	static MessagesTable gs_ReplicaMessages(gs_ReplicaRawMessages, gs_ReplicaRawMessages + ARRAY_SIZE(gs_ReplicaRawMessages));
	return gs_ReplicaMessages;
}

//-----------------------------------//

Enum* ReplicaMessageHandler::getMessagesEnum()
{
	return ReflectionGetType(ReplicaMessageIds);
}

//-----------------------------------//

ReplicaContext* ReplicaMessageHandler::findContext(ReplicaContextId id)
{
	ReplicaContextMap::iterator it = replicaContexts.find(id);

	if( it == replicaContexts.end() )
		return nullptr;

	return it->second;
}

//-----------------------------------//

void ReplicaMessageHandler::handleReplicaObjectUpdate(const SessionPtr& session, const MessagePtr& msg)
{
	uint64 val;
	if( !DecodeVariableInteger(msg->ms, val) )
	{
		LogDebug("ReplicaObjectUpdate: Error decoding context id");
		return;
	}

	ReplicaContextId contextId = (ReplicaContextId) val;
	ReplicaContext* context = findContext(contextId);

	if( !context )
	{
		LogDebug("ReplicaObjectUpdate: Invalid replica context");
		return;
	}

	context->processFieldUpdate(msg);
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END