/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Server/ServerPlugin.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ChatPlugin)

class API_SERVER ChatPlugin : public ServerPlugin
{
	REFLECT_DECLARE_OBJECT(ChatPlugin)

public:

	ChatPlugin();
	~ChatPlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	// Message callbacks.
	void handleClientMessage(const SessionPtr&, const MessagePtr&);
	void handleServerMessage(const SessionPtr&, const MessagePtr&);
	void handleServerAnnouncement(const SessionPtr&, const MessagePtr&);
};

//-----------------------------------//

NAMESPACE_SERVER_END