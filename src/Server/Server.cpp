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
#include "Server/Session.h"

#include "Core/Event.h"
#include "Core/Concurrency.h"
#include "Core/Utilities.h"

#include <enet/enet.h>

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

//-----------------------------------//

Server::Server()
	: tasks(nullptr)
	, networkThread(nullptr)
{
}

//-----------------------------------//

Server::~Server()
{
}

//-----------------------------------//

bool Server::init()
{
	NetworkInitialize();

#if 0
	Log::info("Created %d processing task(s)", Settings::NumTasksProcess);
#endif

	networkThread = ThreadCreate( AllocatorGetServer() );

	host.createSocket("", Settings::HostPort);
	host.onClientConnected.Connect(this, &Server::handleClientConnect);
	host.onClientDisconnected.Connect(this, &Server::handleClientDisconnect);

	return true;
}

//-----------------------------------//

void Server::shutdown()
{
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
	ThreadStart(networkThread, ProcessMessagesThread, &host);
	ThreadSetName(networkThread, "Networking");
	
	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}

	while(true)
	{
		SystemSleep(0);
	}
}

//-----------------------------------//

void Server::parseConfig()
{

}

//-----------------------------------//

void Server::handleClientConnect(const NetworkPeerPtr& networkPeer)
{
	LogInfo("Client connected: %s", networkPeer->getHostName().c_str());

	Session* session = Allocate(Session, AllocatorGetServer());
	session->setPeer(networkPeer);
}

//-----------------------------------//

void Server::handleClientDisconnect(const NetworkPeerPtr& networkPeer)
{
	LogInfo("Client disconnected: %s", networkPeer->getHostName().c_str());
}

//-----------------------------------//

NAMESPACE_SERVER_END