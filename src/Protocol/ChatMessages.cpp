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

REFLECT_ENUM(ChatMessageIds)
	ENUM(ClientMessage)
	ENUM(ServerMessage)
	ENUM(ServerAnnouncement)
REFLECT_ENUM_END()

static MessageMapping gs_ChatRawMessages[] =
{
	{ ChatMessageIds::ClientMessage, MessageDirection::ClientToServer,
		(MessageHandlerMemFn) &ChatMessagePlugin::handleClientMessage },
	
	{ ChatMessageIds::ServerMessage, MessageDirection::ServerToClient,
		(MessageHandlerMemFn) &ChatMessagePlugin::handleServerMessage },
	
	{ ChatMessageIds::ServerAnnouncement, MessageDirection::ServerToClient,
		(MessageHandlerMemFn) &ChatMessagePlugin::handleServerAnnouncement },
};

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ChatMessagePlugin, MessagePlugin)
REFLECT_CLASS_END()

PluginMetadata ChatMessagePlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Chat";
	metadata.description = "Provides chat functionality.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 50;

	return metadata;
}

//-----------------------------------//

const MessagesTable& ChatMessagePlugin::getMessagesTable()
{
	static MessagesTable gs_ChatMessages(gs_ChatRawMessages, gs_ChatRawMessages + ARRAY_SIZE(gs_ChatRawMessages));
	return gs_ChatMessages;
}

//-----------------------------------//

Enum* ChatMessagePlugin::getMessagesEnum()
{
	return ReflectionGetType(ChatMessageIds);
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END