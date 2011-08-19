/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

FWD_DECL_INTRUSIVE(HostClient)
FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

class Dispatcher;
class HostClient;
struct HostConnectionDetails;

NAMESPACE_CORE_END

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ServerPane;

REFLECT_DECLARE_CLASS(ServerPlugin)

class ServerPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(ServerPlugin)

public:

	ServerPlugin();

	GETTER(Host, HostClient*, host)
	GETTER(Dispatcher, Dispatcher*, dispatcher)

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;
	void onPluginUpdate() OVERRIDE;

	// Initializes the network client.
	bool initClient(const HostConnectionDetails&);
	void deinitClient();

	// Connects to the server.
	void connect(const HostConnectionDetails&);

	// Authenticates with the server.
	void auth();

	// Disconnects from the server.
	void disconnect();

	// Checks if the client is connected.
	bool isConnected();

	// Processes the messages.
	void processMessages(Thread* thread, void* userdata);

	// Network callbacks.
	void handleSessionAdded(const SessionPtr&);
	void handleSessionRemoved(const SessionPtr&);

	// Widget callbacks.
	void onConnectClick(wxCommandEvent& event);

	// Keeps the disconnection state.
	bool isDisconnecting;

	HostClient* host;
	Dispatcher* dispatcher;
	Thread* networkThread;
	
	ServerPane* pane;
};

template<typename T> T* GetMessagePlugin()
{
	Dispatcher* dispatcher = GetPlugin<ServerPlugin>()->getDispatcher();
	Plugin* plugin = dispatcher->getPluginManager()->getPluginFromClass(T::getStaticType());
	return (T*) plugin;
}

//-----------------------------------//

NAMESPACE_EDITOR_END