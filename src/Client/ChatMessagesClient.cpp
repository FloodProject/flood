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

class ChatMessagesClient : ChatMessageHandler
{
	void handleChatServer(const SessionPtr&, const ChatServerMessage&) OVERRIDE;
};

REFLECT_CHILD_CLASS(ChatMessagesClient, ChatMessageHandler)
REFLECT_CLASS_END()

//-----------------------------------//

void ChatMessagesClient::handleChatServer(const SessionPtr& session, const ChatServerMessage& message)
{
	LogInfo("Received server message");
}

//-----------------------------------//

NAMESPACE_EDITOR_END