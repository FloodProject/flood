/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Plugin.h"
#include "Network/MessageHandler.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ServerPlugin)

class API_SERVER ServerPlugin : public Plugin, public MessageHandler
{
	REFLECT_DECLARE_OBJECT(ServerPlugin)

	friend class PluginManager;

public:

	ServerPlugin();
	virtual ~ServerPlugin();

	// Updates the plugin.
	virtual void update() {}
};

//-----------------------------------//

NAMESPACE_SERVER_END