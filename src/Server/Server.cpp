/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Server.h"
#include "Server/Settings.h"
#include "Server/ServerPlugin.h"

#include "Core/Event.h"
#include "Core/Concurrency.h"
#include "Core/Utilities.h"
#include "Core/Reflection.h"

#include "Network/Network.h"
#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Dispatcher.h"
#include "Network/SessionManager.h"

#include "Protocol/UserMessages.h"

#include "Engine/Engine.h"

#include <iostream>

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

static Allocator* gs_AllocatorServer = nullptr;

void ServerInitialize()
{
	gs_AllocatorServer = AllocatorCreateHeap(AllocatorGetHeap());
	AllocatorSetGroup(gs_AllocatorServer, "Server");
}

void ServerDeinitialize()
{
	AllocatorDestroy(gs_AllocatorServer);
}

Allocator* AllocatorGetServer()
{
	return gs_AllocatorServer;
}

static Server* gs_ServerInstance = nullptr;
Server* GetServer() { return gs_ServerInstance; }

//-----------------------------------//

Server::Server()
	: taskPool(nullptr)
	, plugins(nullptr)
	, networkThread(nullptr)
	, dispatcher(nullptr)
	, engine(nullptr)
{
	gs_ServerInstance = this;
}

//-----------------------------------//

bool Server::init()
{
	Allocator* alloc = AllocatorGetServer();

	taskPool = TaskPoolCreate(alloc, Settings::NumTasksProcess);

	engine = Allocate(Engine, alloc);
	NetworkInitialize();
	
	host = Allocate(HostServer, alloc);
	
	HostConnectionDetails details;
	details.port = Settings::HostPort;

	if( !host->createSocket(details) )
	{
		LogError("Could not create server socket");
		shutdown();
		return false;
	}
	
	host->onClientConnected.Connect(this, &Server::handleClientConnect);
	host->onClientDisconnected.Connect(this, &Server::handleClientDisconnect);

	dispatcher = Allocate(Dispatcher, alloc);
	dispatcher->initServer(host);
	dispatcher->initPlugins(ReflectionGetType(MessagePlugin));

#ifdef NETWORK_THREAD
	networkThread = ThreadCreate(AllocatorGetServer());
#endif

	std::vector<Plugin*> found;
	plugins = Allocate(PluginManager, alloc);
	plugins->scanPlugins(ReflectionGetType(ServerPlugin), found);
	plugins->sortPlugins(found);
	plugins->registerPlugins(found);

	return true;
}

//-----------------------------------//

void Server::shutdown()
{
	Deallocate(dispatcher);
	Deallocate(host);
#ifdef NETWORK_THREAD
	ThreadDestroy(networkThread);
#endif
	Deallocate(plugins);
	Deallocate(engine);
	NetworkDeinitialize();
	Deallocate(taskPool);
}

//-----------------------------------//
#ifdef NETWORK_THREAD
static void ProcessMessagesThread(Thread* thread, void* data)
{
	Host* host = (Host*) data;
	
	LogInfo("Networking thread will now start listening for messages...");

	while(thread->IsRunning)
	{
		host->processEvents(100);
	}
}
#endif
//-----------------------------------//

void Server::run()
{
#ifdef NETWORK_THREAD
	ThreadStart(networkThread, ProcessMessagesThread, host);
	ThreadSetName(networkThread, "Networking");

	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}
#endif

	while(true)
	{
		SystemSleep(0);

		host->processEvents(10);
		
		while( dispatcher->processMessage() )
			continue;

#if 0
		String input;
		std::getline(std::cin, input);

		if(StringCompareInsensitive(input, "Quit") == 0)
			break;
#endif
	}
}

//-----------------------------------//

void Server::handleClientConnect(const PeerPtr& peer)
{
	String hostname = peer->getHostName().c_str();
	String ip = peer->getHostIP().c_str();

	LogInfo("Client connected: %s (IP: %s)", hostname.c_str(), ip.c_str());
}

//-----------------------------------//

void Server::handleClientDisconnect(const PeerPtr& peer)
{
	String hostname = peer->getHostName().c_str();
	LogInfo("Client disconnected: %s", hostname.c_str());

	const SessionPtr& session = dispatcher->getSessionManager()->getSession(peer);

	UserMessagePlugin* up = GetMessagePlugin<UserMessagePlugin>();
	User* user = up->users.getUserFromSession(session);
	
	if( !user )
	{
		LogDebug("Unauthenticated user disconnecting");
		return;
	}

	UserLeaveMessage leave;
	leave.user = user->id;

	MessagePtr msg = MessageCreate(UserMessageIds::UserLeave);
	msg->write(&leave);
	
	//host->broadcastMessage(msg);
}

//-----------------------------------//

SessionManager* Server::getSessionManager()
{
	return dispatcher->getSessionManager();
}

//-----------------------------------//

NAMESPACE_SERVER_END