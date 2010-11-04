/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Server.h"
#include "Settings.h"

namespace vapor {

//-----------------------------------//

class ProcessMessagesTask : public Task
{
public:

	ProcessMessagesTask(Network& network)
		: network(network)
	{ }
	
	virtual void run()
	{
		while(true)
		{
			network.dispatchMessages();

			if(finish)
				return;
		}
	}

	Network& network;
};

//-----------------------------------//

Server::Server()
	: logger("Server", "Log.html")
	, tasks(Settings::NumThreadsWorkers)
{ }

//-----------------------------------//

Server::~Server()
{
	shutdown();
}

//-----------------------------------//

bool Server::init()
{
	Downloader dl;
	dl.init();
	dl.download("http://www.google.com");

	// Create processing tasks.
	for(int i = 0; i < Settings::NumTasksProcess; i++)
	{
		TaskPtr task = new ProcessMessagesTask(network);
		tasks.addTask(task);
	}

	Log::info("Created %d processing task(s)", Settings::NumTasksProcess);

	network.init();
	network.createServerSocket("tcp://127.0.0.1:7654");

	return true;
}

//-----------------------------------//

void Server::shutdown()
{
	//foreach(Thread*, tasks.getThreads() )
	network.shutdown();
}

//-----------------------------------//

void Server::run()
{
	Thread thread( &Network::waitMessages, boost::ref(network) );
	
	while(true)
	{
		std::string in;
		std::getline(std::cin, in);

		if(in == "quit")
			break;
	}
}

//-----------------------------------//

void Server::parseConfig()
{ }

//-----------------------------------//

} // end namespace