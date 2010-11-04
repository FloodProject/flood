/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Settings.h"

namespace vapor {

//-----------------------------------//

int Settings::NumTasksProcess = 1;
int Settings::NumThreadsWorkers = -1;
int Settings::NumThreadsIO = 1;
std::string Settings::URI = "tcp://*:5555";

//-----------------------------------//

} // end namespace