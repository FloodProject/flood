/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"
#include "Core/Concurrency.h"

FWD_DECL_INTRUSIVE(NetworkClient)
FWD_DECL_INTRUSIVE(NetworkPeer)

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

	// Handles client connections.
	void handleClientConnect(const NetworkPeerPtr& peer);

	// Handles client disconnections.
	void handleClientDisconnect(const NetworkPeerPtr& peer);

	NetworkClientPtr client;
	Thread* networkThread;
};

//-----------------------------------//

NAMESPACE_EDITOR_END