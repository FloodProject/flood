/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/TaskManager.h"
#include "vapor/net/Network.h"
#include "vapor/net/Downloader.h"

namespace vapor {

//-----------------------------------//

class Server
{
public:

	Server();
	~Server();
	
	// Initializes the server.
	bool init();

	// Shutdowns the server.
	void shutdown();

	// Runs the server.
	void run();

	// Parses the config;
	void parseConfig();

protected:

	Logger logger;
	TaskManager tasks;
	Network network;
};

//-----------------------------------//

} // end namespace

using namespace vapor;

int main()
{
	Server server;
	
	server.init();
	server.run();

	return 0;
}