/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Protocol/UserMessages.h"
#include "Server/Server.h"
#include "Network/Message.h"
#include "Network/Dispatcher.h"
#include "Network/Peer.h"
#include "Network/SessionManager.h"
#include "Network/Session.h"
#include "Network/Host.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class UserMessagesServer : UserMessagePlugin
{
	void handleUserAuth(const SessionPtr&, const UserAuthMessage&) OVERRIDE;
	void handleUserChangeName(const SessionPtr&, const UserChangeNameMessage&) OVERRIDE;
};

REFLECT_CHILD_CLASS(UserMessagesServer, UserMessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

void UserMessagesServer::handleUserAuth(const SessionPtr& session, const UserAuthMessage& auth)
{
	User user;
	user.id = users.getNextId();
	user.name = auth.name;
	user.session = session;

	LogDebug("User authenticated: %s", user.name.c_str());

	users.addUser(user);

	// Notify users that another user authenticated.
	UserJoinMessage join;
	join.user = user.id;

	MessagePtr message = MessageCreate(UserMessageIds::UserJoin);
	message->write(&join);

	GetServer()->getHost()->broadcastMessage(message);
}

//-----------------------------------//

void UserMessagesServer::handleUserChangeName(const SessionPtr& session, const UserChangeNameMessage& message)
{

}

//-----------------------------------//

NAMESPACE_SERVER_END