/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Server.h"

using namespace vapor;

int main()
{
	ServerInitialize();
	Log* log = LogCreate( AllocatorGetServer() );

	Server server;
	server.init();
	server.run();
	server.shutdown();

	LogDestroy(log);
	ServerDeinitialize();

	AllocatorDumpInfo();

	return 0;
}