/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Core/Concurrency.h"

FWD_DECL_INTRUSIVE(HostClient)
FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

class HostClient;
class Dispatcher;

NAMESPACE_CORE_END

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(NetworkingPlugin)

class NetworkingPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(NetworkingPlugin)

public:

	NetworkingPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	// Handles client callbacks.
	void handleClientConnect(const PeerPtr&);
	void handleClientDisconnect(const PeerPtr&);

	HostClient* client;
	Dispatcher* dispatcher;
	Thread* networkThread;
};

//-----------------------------------//

NAMESPACE_EDITOR_END