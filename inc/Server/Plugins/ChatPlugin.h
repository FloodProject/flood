/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Server/API.h"
#include "Server/ServerPlugin.h"
#include "Protocol/ChatMessages.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ChatPlugin)

class ChatPlugin : public ServerPlugin, public ChatMessageHandler
{
	REFLECT_DECLARE_OBJECT(ChatPlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	void handleChatClient(const SessionPtr&, const ChatClientMessage&) OVERRIDE;
};

//-----------------------------------//

NAMESPACE_SERVER_END