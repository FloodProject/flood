/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Protocol/API.h"
#include "Protocol/ResourcesMessages.h"
#include "Network/MessageHandlers.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ResourcesAddressMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, string, address)
REFLECT_CLASS_END()

REFLECT_ENUM(ResourcesMessageIds)
	ENUM(ResourcesAddress)
REFLECT_ENUM_END()

IMPLEMENT_HANDLER_REF(Resources, ResourcesAddress)

PROTOCOL_MESSAGE_HANDLERS(Resources)
	HANDLER_REF(Resources, ResourcesAddress, ClientToServer)
PROTOCOL_MESSAGE_HANDLERS_END()

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ResourcesMessageHandler, MessageHandler)
REFLECT_CLASS_END()

//-----------------------------------//

const MessagesTable& ResourcesMessageHandler::getMessagesTable()
{
	static MessagesTable gs_ResourcesMessages(gs_ResourcesRawMessages, gs_ResourcesRawMessages + ARRAY_SIZE(gs_ResourcesRawMessages));
	return gs_ResourcesMessages;
}

//-----------------------------------//

Enum* ResourcesMessageHandler::getMessagesEnum()
{
	return ReflectionGetType(ResourcesMessageIds);
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END