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
#include "Server/Plugins/UserPlugin.h"

#include "Core/Event.h"
#include "Core/Concurrency.h"
#include "Core/Utilities.h"
#include "Core/Reflection.h"

#include "Network/Network.h"
#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/MessageDispatcher.h"
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
	plugins = AllocateThis(PluginManager);

	engine = AllocateThis(Engine);
	NetworkInitialize();
	
	// Initialize host.
	host = AllocateThis(HostServer);
	
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
	
	// Find and instantiate plugins.
	std::vector<Plugin*> found;
	ClassCreateChilds(ReflectionGetType(ServerPlugin), AllocatorGetThis(), found);
	PluginsSortByPriority(found);
	plugins->registerPlugins(found);

	dispatcher = AllocateThis(MessageDispatcher);
	dispatcher->initServer(host);

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
		update();


#if 0
		String input;
		std::getline(std::cin, input);

		if(StringCompareInsensitive(input, "Quit") == 0)
			break;
#endif
	}
}

//-----------------------------------//

void Server::update()
{
	// Update and process networking messages.
	host->processEvents(1);
		
	while( dispatcher->processMessage() )
		continue;

	// Update plugins.
	const std::vector<Plugin*>& plugs = plugins->getPlugins();
	for(size_t i = 0; i < plugs.size(); i++)
	{
		ServerPlugin* plugin = (ServerPlugin*) plugs[i];
		plugin->update();
	}

	SystemSleep(0);
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

	UserPlugin* up = GetPlugin<UserPlugin>();
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