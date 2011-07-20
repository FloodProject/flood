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
#include "Server/SessionManager.h"
#include "Server/ServerPlugin.h"

#include "Core/Event.h"
#include "Core/Concurrency.h"
#include "Core/Utilities.h"

#include <enet/enet.h>

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
	, plugins(nullptr)
	, networkThread(nullptr)
	, host(nullptr)
	, messageHandlers(nullptr)
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

	sessions = Allocate(SessionManager, AllocatorGetServer());
	host = Allocate(NetworkServer, AllocatorGetServer());
	messageHandlers = Allocate(MessageHandlers, AllocatorGetServer());
	networkThread = ThreadCreate(AllocatorGetServer());
	
	initPlugins();

	host->createSocket("", Settings::HostPort);
	host->onClientConnected.Connect(this, &Server::handleClientConnect);
	host->onClientDisconnected.Connect(this, &Server::handleClientDisconnect);
	host->onClientMessage.Connect(this, &Server::handleClientMessage);

	return true;
}

//-----------------------------------//

void Server::shutdown()
{
	Deallocate(plugins);
	Deallocate(sessions);
	Deallocate(messageHandlers);
	Deallocate(host);
	
	ThreadDestroy(networkThread);
	NetworkDeinitialize();
}

//-----------------------------------//

static void ProcessMessagesThread(Thread* thread, void* data)
{
	NetworkHost* host = (NetworkHost*) data;
	
	LogInfo("Networking thread will now start listening for messages...");

	while(thread->IsRunning)
	{
		host->processEvents(100);
	}
}

//-----------------------------------//

void Server::run()
{
	ThreadStart(networkThread, ProcessMessagesThread, host);
	ThreadSetName(networkThread, "Networking");

	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}

	while(true)
	{
		SystemSleep(0);

		String input;
		std::getline(std::cin, input);

		if(StringCompareInsensitive(input, "Quit") == 0)
			break;
	}
}

//-----------------------------------//

void Server::initPlugins()
{
	plugins = Allocate(PluginManager, AllocatorGetServer());
	plugins->onPluginEnableEvent.Connect(this, &Server::handlePluginEnable);
	plugins->onPluginDisableEvent.Connect(this, &Server::handlePluginDisable);

	std::vector<Plugin*> found;
	plugins->scanPlugins(ReflectionGetType(ServerPlugin), found);
	plugins->sortPlugins(found);
	plugins->registerPlugins(found);
}

//-----------------------------------//

void Server::handlePluginEnable(Plugin* plugin)
{
	ServerPlugin* serverPlugin = (ServerPlugin*) plugin;

	const MessagesTable& messagesTable = serverPlugin->getMessagesTable();
	Enum* messagesEnum = serverPlugin->getMessagesEnum();

	for(size_t i = 0; i < messagesTable.size(); i++ )
	{
		MessageHandler handler = messagesTable[i];
		handler.plugin = serverPlugin;

		messageHandlers->addHandler(handler);

		const char* name = EnumGetValueName(messagesEnum, handler.id);
		LogDebug("Registering message type: %s", name);
	}
}

//-----------------------------------//

void Server::handlePluginDisable(Plugin* plugin)
{
	ServerPlugin* serverPlugin = (ServerPlugin*) plugin;

	const MessagesTable& messagesTable = serverPlugin->getMessagesTable();
	Enum* messagesEnum = serverPlugin->getMessagesEnum();

	for(size_t i = 0; i < messagesTable.size(); i++ )
	{
		const MessageHandler& handler = messagesTable[i];
		messageHandlers->addHandler(handler);
	}
}

//-----------------------------------//

void Server::handleClientConnect(const NetworkPeerPtr& networkPeer)
{
	SessionPtr session = Allocate(Session, AllocatorGetServer());
	session->setPeer(networkPeer);
	sessions->addSession(session);

	String hostname = networkPeer->getHostName().c_str();
	String ip = networkPeer->getHostIP().c_str();

	LogInfo("Client connected: %s (IP: %s)", hostname.c_str(), ip.c_str());
}

//-----------------------------------//

void Server::handleClientDisconnect(const NetworkPeerPtr& networkPeer)
{
	const SessionPtr& session = sessions->getSession(networkPeer);
	assert( session != nullptr );

	sessions->removeSession(session);

	LogInfo("Client disconnected: %s", networkPeer->getHostName().c_str());
}

//-----------------------------------//

void Server::handleClientMessage(const NetworkPeerPtr& networkPeer, const MessagePtr& message)
{
	const SessionPtr& session = sessions->getSession(networkPeer);
	assert( session != nullptr );
}

//-----------------------------------//

NAMESPACE_SERVER_END