/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

struct Settings
{
	// Number of processing tasks.
	static int NumTasksProcess;

	// Number of worker threads.
	static int NumThreadsWorkers;

	// Number of I/O threads.
	static int NumThreadsIO;

	// Host location.
	static String HostLocation;

	// Host port.
	static int HostPort;

	// Resources port.
	static int ResourcesPort;

	// Resources path.
	static Path ResourcesPath;
};

//-----------------------------------//

NAMESPACE_SERVER_END