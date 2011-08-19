/************************************************************************
*
* vapor3D Server � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Log.h"
#include "Core/Concurrency.h"
#include "Core/References.h"
#include "Core/PluginManager.h"

FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class Plugin;
class HostServer;
class Dispatcher;
class SessionManager;
class Engine;

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

	// Gets the session manager.
	SessionManager* getSessionManager();

	// Gets the host.
	GETTER(Host, HostServer*, host)

	// Handles client callbacks.
	void handleClientConnect(const PeerPtr&);
	void handleClientDisconnect(const PeerPtr&);

	TaskPool* taskPool;
	PluginManager* plugins;
	Engine* engine;

	HostServer* host;
	Dispatcher* dispatcher;
	Thread* networkThread;
};

API_SERVER Server* GetServer();

template<typename T> T* GetPlugin()
{
	PluginManager* pm = GetServer()->plugins;
	return (T*) pm->getPluginFromClass(T::getStaticType());
}

template<typename T> T* GetMessagePlugin()
{
	PluginManager* pm = GetServer()->dispatcher->getPluginManager();
	return (T*) pm->getPluginFromClass(T::getStaticType());
}

//-----------------------------------//

NAMESPACE_SERVER_END