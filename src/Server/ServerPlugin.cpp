/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/ServerPlugin.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ServerPlugin, MessagePlugin)
REFLECT_CLASS_END()

//-----------------------------------//

ServerPlugin::ServerPlugin()
{
}

//-----------------------------------//

ServerPlugin::~ServerPlugin()
{
}

//-----------------------------------//

NAMESPACE_SERVER_END