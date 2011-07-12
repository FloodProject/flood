/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Settings.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

int Settings::NumTasksProcess = 1;
int Settings::NumThreadsWorkers = -1;
int Settings::NumThreadsIO = 1;
String Settings::URI = "tcp://*:5555";

//-----------------------------------//

NAMESPACE_SERVER_END