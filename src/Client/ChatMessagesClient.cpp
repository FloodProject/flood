/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Protocol/ChatMessages.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ChatMessagesClient : ChatMessagePlugin
{
	void handleServerMessage(const SessionPtr&, const MessagePtr&) OVERRIDE;
	void handleServerAnnouncement(const SessionPtr&, const MessagePtr&) OVERRIDE;
};

REFLECT_CHILD_CLASS(ChatMessagesClient, ChatMessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

void ChatMessagesClient::handleServerMessage(const SessionPtr& session, const MessagePtr& message)
{
	LogInfo("Received server message");
}

//-----------------------------------//

void ChatMessagesClient::handleServerAnnouncement(const SessionPtr& session, const MessagePtr& message)
{
	LogInfo("Received server announcement");
}

//-----------------------------------//

NAMESPACE_EDITOR_END