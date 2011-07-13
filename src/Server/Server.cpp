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

#include <enet/enet.h>

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

static Allocator* g_AllocatorServer = nullptr;

void InitializeServerAllocator()
{
	g_AllocatorServer = AllocatorCreateHeap(AllocatorGetHeap(), "Server");
}

Allocator* AllocatorGetServer()
{
	return g_AllocatorServer;
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

	return true;
}

//-----------------------------------//

void Server::shutdown()
{
	ThreadDestroy(networkThread);

	NetworkDeinitialize();
}

//-----------------------------------//

static void ProcessMessagesThread(void* threadUserdata)
{
	NetworkHost* host = (NetworkHost*) threadUserdata;
	
	LogInfo("Networking thread will now start listening for messages...");

	while(true)
	{
		host->waitMessages();
		host->dispatchMessages();
	}
}

//-----------------------------------//

void Server::run()
{
	ThreadStart(networkThread, ProcessMessagesThread, &host);
	ThreadSetName(networkThread, "Network");
	
	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}

	while(true)
	{
		char buf[80];
		if(strcmp(gets(buf), "quit\n"))
			return;
	}
}

//-----------------------------------//

void Server::parseConfig()
{

}

//-----------------------------------//

void Server::handleClientConnect(const NetworkPeer& networkPeer)
{
	LogInfo("Client connected: %s", networkPeer.getHostname().c_str());
}

//-----------------------------------//

void Server::handleClientDisconnect(const NetworkPeer& peer)
{
}

//-----------------------------------//

NAMESPACE_SERVER_END