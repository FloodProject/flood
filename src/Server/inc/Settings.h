/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class Settings
{
public:

	// Number of processing tasks.
	static int NumTasksProcess;

	// Number of worker threads.
	static int NumThreadsWorkers;

	// Number of I/O threads.
	static int NumThreadsIO;

	// Server URI.
	static std::string URI;
};

//-----------------------------------//

} // end namespace