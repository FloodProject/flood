/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Server.h"

#ifdef NAMESPACE_CORE_DEFINED
using namespace NAMESPACE_CORE;
#endif

int main()
{
	int exitCode = EXIT_SUCCESS;

	ServerInitialize();
	Log* log = LogCreate( AllocatorGetServer() );

	Server server;

	if( !server.init() )
	{
		exitCode = EXIT_FAILURE;
		goto exit;
	}

	server.run();
	server.shutdown();

exit:

	LogDestroy(log);
	ServerDeinitialize();

	AllocatorDumpInfo();

	return exitCode;
}