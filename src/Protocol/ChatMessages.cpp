/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Protocol/API.h"
#include "Protocol/ChatMessages.h"
#include "Network/MessageHandlers.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ChatClientMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, string, text)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ChatServerMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, uint8, user)
	FIELD_PRIMITIVE(1, string, text)
REFLECT_CLASS_END()

REFLECT_ENUM(ChatMessageIds)
	ENUM(ChatClient)
	ENUM(ChatServer)
REFLECT_ENUM_END()

IMPLEMENT_HANDLER_REF(Chat, ChatClient)
IMPLEMENT_HANDLER_REF(Chat, ChatServer)

PROTOCOL_MESSAGE_HANDLERS(Chat)
	HANDLER_REF(Chat, ChatClient, ClientToServer)
	HANDLER_REF(Chat, ChatServer, ServerToClient)
PROTOCOL_MESSAGE_HANDLERS_END()

REFLECT_ABSTRACT_CHILD_CLASS(ChatMessageHandler, MessageHandler)
REFLECT_CLASS_END()

//-----------------------------------//

const MessagesTable& ChatMessageHandler::getMessagesTable()
{
	static MessagesTable gs_ChatMessages(gs_ChatRawMessages, gs_ChatRawMessages + ARRAY_SIZE(gs_ChatRawMessages));
	return gs_ChatMessages;
}

//-----------------------------------//

Enum* ChatMessageHandler::getMessagesEnum()
{
	return ReflectionGetType(ChatMessageIds);
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END