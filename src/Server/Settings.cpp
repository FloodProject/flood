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
String Settings::HostLocation = "127.0.0.1";
int Settings::HostPort = 9999;
int Settings::ResourcesPort = 9998;
Path Settings::ResourcesPath = "Assets";

//-----------------------------------//

NAMESPACE_SERVER_END