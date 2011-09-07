/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Plugins/ChatPlugin.h"
#include "Server/Plugins/UserPlugin.h"
#include "Network/Message.h"
#include "Network/Dispatcher.h"
#include "Server/Server.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ChatPlugin, ServerPlugin)
REFLECT_CLASS_END()

PROTOCOL_PLUGIN_BEGIN(ChatPlugin)
	METADATA_NAME(Chat)
	METADATA_DESC(Provides chat functionality.)
	METADATA_AUTHOR(triton)
	METADATA_VERSION(1.0)
	METADATA_PRIORITY(50)
PROTOCOL_PLUGIN_END()

//-----------------------------------//

void ChatPlugin::handleChatClient(const SessionPtr& session, const ChatClientMessage& chat)
{
	UserPlugin* usersPlugin = GetPlugin<UserPlugin>();
	User* user = usersPlugin->users.getUserFromSession(session);
	
	LogInfo("%s says: %s", user->name.c_str(), chat.text.c_str());
}

//-----------------------------------//

NAMESPACE_SERVER_END