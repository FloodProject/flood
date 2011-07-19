/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "NetworkingPlugin.h"
#include "Network/Network.h"
#include "Network/Message.h"
#include "Editor.h"
#include "Settings.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(NetworkingPlugin, Plugin)
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
	NetworkClient* client = (NetworkClient*) userdata;

	String address(HostAddress);

	LogInfo("Attempting to connect to server...");

	client->connect(address, HostPort);
	client->processEvents(500);

	MessageData data;
	data.push_back(0x10);

	MessagePtr message = Allocate(Message, AllocatorGetHeap());
	message->setData(data);
	message->prepare();

	client->getPeer()->queueMessage(message, 0);

	while(thread->IsRunning)
	{
		client->processEvents(100);
	}
}

//-----------------------------------//

void NetworkingPlugin::onPluginEnable()
{
	client = nullptr;
	networkThread = nullptr;

	client = Allocate(NetworkClient, AllocatorGetHeap());
	client->onClientConnected.Connect(this, &NetworkingPlugin::handleClientConnect);
	client->onClientDisconnected.Connect(this, &NetworkingPlugin::handleClientDisconnect);

	networkThread = ThreadCreate( AllocatorGetHeap() );
	ThreadStart(networkThread, ProcessMessagesThread, client.get());
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

	client.reset();
}

//-----------------------------------//

void NetworkingPlugin::handleClientConnect(const NetworkPeerPtr& networkPeer)
{
	LogInfo("Connected to server at: %s", networkPeer->getHostIP().c_str());
}

//-----------------------------------//

void NetworkingPlugin::handleClientDisconnect(const NetworkPeerPtr& peer)
{
}


//-----------------------------------//

NAMESPACE_EDITOR_END