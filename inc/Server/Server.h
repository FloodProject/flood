/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Log.h"
#include "Core/Concurrency.h"
#include "Core/PluginManager.h"
#include "Network/Network.h"
#include "Network/MessageHandlers.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

API_SERVER void ServerInitialize();
API_SERVER void ServerDeinitialize();

class SessionManager;

class Server
{
public:

	Server();
	
	// Initializes the server.
	bool init();

	// Shutdowns the server.
	void shutdown();

	// Runs the server.
	void run();

	// Initializes the plugins.
	void initPlugins();

	// Gets the plugin manager.
	GETTER(PluginManager, PluginManager*, plugins)

	// Handles plugins callbacks.
	void handlePluginEnable(Plugin*);
	void handlePluginDisable(Plugin*);

	// Handles client callbacks.
	void handleClientConnect(const NetworkPeerPtr&);
	void handleClientDisconnect(const NetworkPeerPtr&);
	void handleClientMessage(const NetworkPeerPtr&, const MessagePtr&);

protected:

	TaskPool* tasks;
	PluginManager* plugins;
	Thread* networkThread;
	NetworkServer* host;
	SessionManager* sessions;
	MessageHandlers* messageHandlers;
};

API_SERVER Server* GetServer();

//-----------------------------------//

NAMESPACE_SERVER_END