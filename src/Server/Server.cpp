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

#include "Core/Event.h"
#include "Core/Concurrency.h"
#include "Core/Utilities.h"

#include "Network/Network.h"
#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Dispatcher.h"
#include "Network/SessionManager.h"

#include "Protocol/UserMessages.h"

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
	: tasks(nullptr)
	, networkThread(nullptr)
	, dispatcher(nullptr)
{
	gs_ServerInstance = this;
}

//-----------------------------------//

bool Server::init()
{
	NetworkInitialize();

#if 0
	Log::info("Created %d processing task(s)", Settings::NumTasksProcess);
#endif

	host = Allocate(HostServer, AllocatorGetServer());
	
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

	dispatcher = Allocate(Dispatcher, AllocatorGetServer());
	dispatcher->initServer(host);
	dispatcher->initPlugins(ReflectionGetType(MessagePlugin));

#ifdef NETWORK_THREAD
	networkThread = ThreadCreate(AllocatorGetServer());
#endif

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
	NetworkDeinitialize();
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

	UserMessagePlugin* up = GetPlugin<UserMessagePlugin>();
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