/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

FWD_DECL_INTRUSIVE(NetworkClient)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(NetworkingPlugin)

class NetworkingPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(NetworkingPlugin)

public:

	NetworkingPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Connects to the remote host.
	void connect();

	NetworkClientPtr client;
};

//-----------------------------------//

NAMESPACE_EDITOR_END