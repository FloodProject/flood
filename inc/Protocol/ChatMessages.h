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

struct API_PROTOCOL ChatMessage
{
	String text;
};

#define CHAT_INITIAL_ID 200

REFLECT_DECLARE_ENUM(ChatMessageIds)

struct API_PROTOCOL ChatMessageIds
{
	enum Enum : MessageId
	{
		ClientMessage = CHAT_INITIAL_ID + 0,
		ServerMessage = CHAT_INITIAL_ID + 1,
		ServerAnnouncement = CHAT_INITIAL_ID +2
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
	virtual void handleClientMessage(const SessionPtr&, const MessagePtr&) {}
	virtual void handleServerMessage(const SessionPtr&, const MessagePtr&) {}
	virtual void handleServerAnnouncement(const SessionPtr&, const MessagePtr&) {}
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END