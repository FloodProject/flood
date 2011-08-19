/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Plugin.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ServerPlugin)

class API_SERVER ServerPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(ServerPlugin)

	friend class PluginManager;

public:

	ServerPlugin();
	virtual ~ServerPlugin();
};

//-----------------------------------//

NAMESPACE_SERVER_END