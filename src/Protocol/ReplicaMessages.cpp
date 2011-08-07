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

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ReplicaCreateInstanceMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, uint16, localId)
	FIELD_PRIMITIVE(1, uint16, classId)
	FIELD_CLASS_PTR(2, Object, Object*, instance, RawPointer)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ReplicaNewInstanceMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, uint64, instanceId)
	FIELD_CLASS_PTR(2, Object, Object*, instance, RawPointer)
REFLECT_CLASS_END()

REFLECT_ENUM(ReplicaMessageIds)
	ENUM(ReplicaCreateInstance)
	ENUM(ReplicaNewInstance)
REFLECT_ENUM_END()

IMPLEMENT_HANDLER_REF(Replica, ReplicaCreateInstance)
IMPLEMENT_HANDLER_REF(Replica, ReplicaNewInstance)

PROTOCOL_MESSAGE_HANDLERS(Replica)
	HANDLER_REF(Replica, ReplicaCreateInstance, ClientToServer)
	HANDLER_REF(Replica, ReplicaNewInstance, ServerToClient)
PROTOCOL_MESSAGE_HANDLERS_END()

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ReplicaMessagePlugin, MessagePlugin)
REFLECT_CLASS_END()

PROTOCOL_PLUGIN_BEGIN(ReplicaMessagePlugin)
	METADATA_NAME(Replica)
	METADATA_DESC(Provides object replication functionality.)
	METADATA_AUTHOR(triton)
	METADATA_VERSION(1.0)
	METADATA_PRIORITY(20)
PROTOCOL_PLUGIN_END()

//-----------------------------------//

ReplicaMessagePlugin::ReplicaMessagePlugin()
	: nextInstance(0)
{
}

//-----------------------------------//

const MessagesTable& ReplicaMessagePlugin::getMessagesTable()
{
	static MessagesTable gs_ReplicaMessages(gs_ReplicaRawMessages, gs_ReplicaRawMessages + ARRAY_SIZE(gs_ReplicaRawMessages));
	return gs_ReplicaMessages;
}

//-----------------------------------//

Enum* ReplicaMessagePlugin::getMessagesEnum()
{
	return ReflectionGetType(ReplicaMessageIds);
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END