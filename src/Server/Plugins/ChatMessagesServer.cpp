/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Protocol/ChatMessages.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class ChatMessagesServer : ChatMessagePlugin
{
	void handleClientMessage(const SessionPtr&, const MessagePtr&) OVERRIDE;
};

REFLECT_CHILD_CLASS(ChatMessagesServer, ChatMessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

void ChatMessagesServer::handleClientMessage(const SessionPtr& session, const MessagePtr& message)
{
	LogInfo("Received client message");
}

//-----------------------------------//

NAMESPACE_SERVER_END