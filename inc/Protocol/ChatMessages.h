/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"
#include "Protocol/UserMessages.h"
#include "Network/MessagePlugin.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ChatClientMessage)
struct API_PROTOCOL ChatClientMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ChatClientMessage)
	String text;
};

REFLECT_DECLARE_CLASS(ChatServerMessage)
struct API_PROTOCOL ChatServerMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ChatServerMessage)
	UserId user;
	String text;
};

#define CHAT_INITIAL_ID 200

REFLECT_DECLARE_ENUM(ChatMessageIds)

struct API_PROTOCOL ChatMessageIds
{
	enum Enum : MessageId
	{
		ChatClient = CHAT_INITIAL_ID + 0,
		ChatServer = CHAT_INITIAL_ID + 1,
	};
};

REFLECT_DECLARE_CLASS(ChatMessagePlugin)

//-----------------------------------//

class API_PROTOCOL ChatMessagePlugin : public MessagePlugin
{
	REFLECT_DECLARE_OBJECT(ChatMessagePlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Message callbacks.
	virtual void handleChatClient(const SessionPtr&, const ChatClientMessage&) {}
	virtual void handleChatServer(const SessionPtr&, const ChatServerMessage&) {}
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END