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

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

Allocator* AllocatorGetServer()
{
	static Allocator* alloc = AllocatorCreateHeap(AllocatorGetHeap(), "Server");
	return alloc;
}

//-----------------------------------//

Server::Server()
	: tasks(nullptr)
{
}

//-----------------------------------//

Server::~Server()
{
	shutdown();
}

//-----------------------------------//

bool Server::init()
{
#if 0
	Log::info("Created %d processing task(s)", Settings::NumTasksProcess);
#endif

	host.createSocket("", 38092);

	return true;
}

//-----------------------------------//

void Server::shutdown()
{

}

//-----------------------------------//

static void ProcessMessagesThread(void* threadUserdata)
{
	NetworkHost* host = (NetworkHost*) threadUserdata;
	
	LogInfo("Networking thread will now start listening for messages...");

	while(true)
	{
		host->dispatchMessages();
	}
}

//-----------------------------------//

void Server::run()
{
	Thread* networkThread = ThreadCreate( AllocatorGetServer() );
	ThreadStart(networkThread, ProcessMessagesThread, &host);
	
	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}

	LogInfo("Created networking thread");
	 
	String in;
	
	while(true)
	{
		char buf[80];
		fgets(buf, ARRAY_SIZE(buf), stdin);
		in.assign(buf);

		if(in == "quit")
			break;
	}

	ThreadDestroy(networkThread);
}

//-----------------------------------//

void Server::parseConfig()
{

}

//-----------------------------------//

NAMESPACE_SERVER_END

using namespace vapor;

int main()
{
	Log* log = LogCreate( AllocatorGetServer() );

	Server server;
	
	server.init();
	server.run();

	LogDestroy(log);

	return 0;
}