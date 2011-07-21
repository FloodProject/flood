/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "NetworkingPlugin.h"
#include "Network/Host.h"
#include "Network/Message.h"
#include "Network/Dispatcher.h"
#include "Network/MessagePlugin.h"
#include "Editor.h"
#include "Settings.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(NetworkingPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

NetworkingPlugin::NetworkingPlugin()
{

}

//-----------------------------------//

PluginMetadata NetworkingPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Networking";
	metadata.description = "Handles connecting to remote hosts.";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

static void ProcessMessagesThread(Thread* thread, void* userdata)
{
	HostClient* client = (HostClient*) userdata;

	String address(HostAddress);

	LogInfo("Attempting to connect to server...");

	client->connect(address, HostPort);
	client->processEvents(500);

	while(thread->IsRunning)
	{
		client->processEvents(100);
	}
}

//-----------------------------------//

void NetworkingPlugin::onPluginEnable()
{
	client = Allocate(HostClient, AllocatorGetThis());
	client->onClientConnected.Connect(this, &NetworkingPlugin::handleClientConnect);
	client->onClientDisconnected.Connect(this, &NetworkingPlugin::handleClientDisconnect);

	dispatcher = Allocate(Dispatcher, AllocatorGetThis());
	dispatcher->initClient(client);
	dispatcher->initPlugins(ReflectionGetType(MessagePlugin));

	networkThread = ThreadCreate( AllocatorGetThis() );
	ThreadStart(networkThread, ProcessMessagesThread, client);
	ThreadSetName(networkThread, "Networking");
	
	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}
}

//-----------------------------------//

void NetworkingPlugin::onPluginDisable()
{
	PluginMetadata metadata = getMetadata();

	networkThread->IsRunning = false;
	ThreadJoin(networkThread);
	ThreadDestroy(networkThread);
	networkThread = nullptr;

	Deallocate(client);
	client = nullptr;

	Deallocate(dispatcher);
	dispatcher = nullptr;
}

//-----------------------------------//

void NetworkingPlugin::handleClientConnect(const PeerPtr& networkPeer)
{
	LogInfo("Connected to server at: %s", networkPeer->getHostIP().c_str());
}

//-----------------------------------//

void NetworkingPlugin::handleClientDisconnect(const PeerPtr& peer)
{
}

//-----------------------------------//

NAMESPACE_EDITOR_END