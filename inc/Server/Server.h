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
#include "Network/Network.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class Server
{
public:

	Server();
	~Server();
	
	// Initializes the server.
	bool init();

	// Shutdowns the server.
	void shutdown();

	// Runs the server.
	void run();

	// Parses the config;
	void parseConfig();

	// Handles client connections.
	void handleClientConnect(const NetworkPeer& peer);

	// Handles client disconnections.
	void handleClientDisconnect(const NetworkPeer& peer);

protected:

	TaskPool* tasks;
	Thread* networkThread;
	NetworkHost host;
};

API_SERVER Allocator* AllocatorGetServer();

//-----------------------------------//

NAMESPACE_SERVER_END
