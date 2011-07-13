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
	InitializeServerAllocator();
	Log* log = LogCreate( AllocatorGetServer() );

	Server server;
	server.init();
	server.run();
	server.shutdown();

	LogDestroy(log);
	AllocatorDestroy( AllocatorGetServer() );

	AllocatorDumpInfo();

	return 0;
}