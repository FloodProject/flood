/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Protocol/ChatMessages.h"
#include "Protocol/UserMessages.h"
#include "Protocol/Users.h"
#include "Network/Message.h"
#include "Network/Dispatcher.h"
#include "Server/Server.h"

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
	ChatMessage* chat = message->read<ChatMessage>();
	String text( chat->text );

	UserMessagePlugin* usersPlugin = GetPlugin<UserMessagePlugin>();
	User* user = usersPlugin->users.getUserFromSession(session);
	
	LogInfo("%s says: %s", user->name.c_str(), text.c_str());
}


//-----------------------------------//

NAMESPACE_SERVER_END