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
#include "Core/ReferenceCount.h"

FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class Plugin;
class HostServer;
class Dispatcher;

API_SERVER void ServerInitialize();
API_SERVER void ServerDeinitialize();

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

	// Handles client callbacks.
	void handleClientConnect(const PeerPtr&);
	void handleClientDisconnect(const PeerPtr&);

protected:

	TaskPool* tasks;
	HostServer* host;
	Dispatcher* dispatcher;
	Thread* networkThread;
};

API_SERVER Server* GetServer();

//-----------------------------------//

NAMESPACE_SERVER_END