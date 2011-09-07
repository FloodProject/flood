/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Plugins/UserPlugin.h"
#include "Server/Server.h"
#include "Protocol/UserMessages.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(UserPlugin, ServerPlugin)
REFLECT_CLASS_END()

PROTOCOL_PLUGIN_BEGIN(UserPlugin)
	METADATA_NAME(User)
	METADATA_DESC(Provides user management functionality.)
	METADATA_AUTHOR(triton)
	METADATA_VERSION(1.0)
	METADATA_PRIORITY(30)
PROTOCOL_PLUGIN_END()

//-----------------------------------//

void UserPlugin::handleUserAuth(const SessionPtr& session, const UserAuthMessage& auth)
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
	join.name = user.name;

	MessagePtr m_join = MessageCreate(UserMessageIds::UserJoin);
	m_join->write(&join);
	GetServer()->getHost()->broadcastMessage(m_join);

	UserAuthStatusMessage status;

	MessagePtr m_status = MessageCreate(UserMessageIds::UserAuthStatus);
	m_status->write(&status);
	session->getPeer()->queueMessage(m_status, 0);
}

//-----------------------------------//

void UserPlugin::handleUserChangeName(const SessionPtr& session, const UserChangeNameMessage& message)
{

}

//-----------------------------------//

NAMESPACE_SERVER_END