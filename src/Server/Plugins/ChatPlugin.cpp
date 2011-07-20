/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Plugins/ChatPlugin.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ChatPlugin, ServerPlugin)
REFLECT_CLASS_END()

#define CHAT_INITIAL_ID 200

struct ChatMessageIds
{
	enum Enum : MessageId
	{
		ClientMessage = CHAT_INITIAL_ID + 0,
		ServerMessage = CHAT_INITIAL_ID + 1,
		ServerAnnouncement = CHAT_INITIAL_ID +2
	};
};

REFLECT_DECLARE_ENUM(ChatMessageIds)

REFLECT_ENUM(ChatMessageIds)
	ENUM(ClientMessage)
	ENUM(ServerMessage)
	ENUM(ServerAnnouncement)
REFLECT_ENUM_END()

static MessageHandler gs_ChatRawMessages[] =
{
	{ ChatMessageIds::ClientMessage, (MessageHandlerMemFn) &ChatPlugin::handleClientMessage, nullptr },
	{ ChatMessageIds::ServerMessage, (MessageHandlerMemFn) &ChatPlugin::handleServerMessage, nullptr },
	{ ChatMessageIds::ServerAnnouncement, (MessageHandlerMemFn) &ChatPlugin::handleServerAnnouncement, nullptr },
};

//-----------------------------------//

ChatPlugin::ChatPlugin()
{
}

//-----------------------------------//

ChatPlugin::~ChatPlugin()
{
}

//-----------------------------------//

PluginMetadata ChatPlugin::getMetadata()
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

const MessagesTable& ChatPlugin::getMessagesTable()
{
	static MessagesTable gs_ChatMessages(gs_ChatRawMessages, gs_ChatRawMessages + ARRAY_SIZE(gs_ChatRawMessages));
	return gs_ChatMessages;
}

//-----------------------------------//

Enum* ChatPlugin::getMessagesEnum()
{
	return ReflectionGetType(ChatMessageIds);
}

//-----------------------------------//

void ChatPlugin::onPluginEnable()
{
}

//-----------------------------------//

void ChatPlugin::onPluginDisable()
{
}

//-----------------------------------//

void ChatPlugin::handleClientMessage(const SessionPtr& session, const MessagePtr& message)
{
}

//-----------------------------------//

void ChatPlugin::handleServerMessage(const SessionPtr& session, const MessagePtr& message)
{
}

//-----------------------------------//

void ChatPlugin::handleServerAnnouncement(const SessionPtr& session, const MessagePtr& message)
{
}

//-----------------------------------//

NAMESPACE_SERVER_END