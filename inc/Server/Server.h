/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Log.h"
#include "Core/Concurrency.h"
#include "Net/Network.h"

NAMESPACE_SERVER_BEGIN

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

	TaskPool* tasks;
	NetworkHost host;
};

API_SERVER Allocator* AllocatorGetServer();

//-----------------------------------//

NAMESPACE_SERVER_END
