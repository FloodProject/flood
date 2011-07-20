/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#include "Core/Log.h"
#include "Core/Memory.h"

#include "Network/Network.h"
#include "Network/Message.h"

#include <enet/enet.h>

NAMESPACE_BEGIN

//-----------------------------------//

static Allocator* gs_NetworkAllocator = nullptr;

static void* ENET_CALLBACK enet_custom_malloc(size_t size)
{
	return AllocatorAllocate(gs_NetworkAllocator, size, 0);
}

static void ENET_CALLBACK enet_custom_free(void * memory)
{
	return AllocatorDeallocate(memory);
}

//-----------------------------------//

bool NetworkInitialize()
{
	gs_NetworkAllocator = AllocatorCreateHeap(AllocatorGetHeap());
	AllocatorSetGroup(gs_NetworkAllocator, "Network");

	ENetCallbacks callbacks;
	callbacks.malloc = enet_custom_malloc;
	callbacks.free = enet_custom_free;

	int ret = enet_initialize_with_callbacks(ENET_VERSION, &callbacks);

	if(ret != 0)
	{
		LogError("Error initializing ENet");
		return false;
	}

	LogInfo("Initialized ENet %d.%d.%d networking layer",
		ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH);

	return true;
}

//-----------------------------------//

void NetworkDeinitialize()
{
	enet_deinitialize();
	
	AllocatorDestroy(gs_NetworkAllocator);
	gs_NetworkAllocator = nullptr;

	LogInfo("Deinitialized ENet");
}

//-----------------------------------//

#define ENET_BANDWIDTH_AUTO 0

static ENetHost* CreateEnetSocket( ENetAddress* address )
{
	int numClients = 32;
	int numChannels = 2;
	int numBandwidthIn = ENET_BANDWIDTH_AUTO;
	int numBandwidthOut = ENET_BANDWIDTH_AUTO;
	
	ENetHost* host = enet_host_create(address,
		numClients, numChannels, numBandwidthIn, numBandwidthOut);

	if( !host )
	{
		LogError("Error creating ENet host");
		return nullptr;
	}

	return host;
}

//-----------------------------------//

NetworkPeer::NetworkPeer()
	: peer(nullptr)
{
}

//-----------------------------------//

void NetworkPeer::queueMessage(const MessagePtr& message, uint8 channel)
{
	if( !message ) return;
	message->prepare();
	enet_peer_send(peer, channel, message->getPacket());
}

//-----------------------------------//

String NetworkPeer::getHostName() const
{
	char name[256];
	
	if(enet_address_get_host(&peer->address, name, ARRAY_SIZE(name)) < 0)
	{
		LogError("Could not get hostname of network peer");
		return "";
	}

	return String(name);
}

//-----------------------------------//

String NetworkPeer::getHostIP() const
{
	char ip[64];
	
	if(enet_address_get_host_ip(&peer->address, ip, ARRAY_SIZE(ip)) < 0)
	{
		LogError("Could not get IP address of network peer");
		return "";
	}

	return String(ip);
}

//-----------------------------------//

NetworkHost::NetworkHost()
	: host(nullptr)
{
}

//-----------------------------------//

NetworkHost::~NetworkHost()
{
	enet_host_destroy(host);
}

//-----------------------------------//

void NetworkHost::processEvents(uint32 timeout)
{
	ENetEvent event;
	
	while(enet_host_service(host, &event, timeout) > 0)
	{
		switch(event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			handleConnectEvent(&event);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			handleDisconnectEvent(&event);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			handleReceiveEvent(&event);
			break;
		};
	}
}

//-----------------------------------//

void NetworkHost::handleConnectEvent(ENetEvent* event)
{
	ENetPeer* peer = event->peer;

	NetworkPeerPtr networkPeer = Allocate(NetworkPeer, AllocatorGetHeap());
	networkPeer->peer = peer;

	// Store the network peer as user data.
	peer->data = networkPeer.get();

	onConnected(networkPeer);
}

//-----------------------------------//

void NetworkHost::handleDisconnectEvent(ENetEvent* event)
{
	NetworkPeerPtr networkPeer = (NetworkPeer*) event->peer->data;
	
	onDisconnected(networkPeer);

	// Reset the peer userdata.
	ENetPeer* peer = event->peer;
	peer->data = nullptr;
}

//-----------------------------------//

void NetworkHost::handleReceiveEvent(ENetEvent* event)
{
	NetworkPeerPtr peer = (NetworkPeer*) event->peer->data;
	
	MessagePtr message = Allocate(Message, gs_NetworkAllocator);
	message->setPacket(event->packet);

	onMessage(peer, message);
}

//-----------------------------------//

NetworkClient::NetworkClient()
{
	state = NetworkClientState::Initial;
}

//-----------------------------------//

bool NetworkClient::connect( const String& address, int port )
{
	state = NetworkClientState::Connecting;

	host = CreateEnetSocket(nullptr);

	if( !host )
		return false;

	ENetAddress addr;
	addr.host = 0;
	addr.port = port;

	enet_address_set_host( &addr, address.c_str() );

	size_t channelCount = 2;
	enet_uint32 data = 0;

	ENetPeer* newPeer = enet_host_connect(host, &addr, channelCount, data);

	peer = Allocate(NetworkPeer, gs_NetworkAllocator);
	peer->peer = newPeer;

	return true;
}

//-----------------------------------//

void NetworkClient::onConnected(const NetworkPeerPtr& newPeer)
{
	peer = newPeer;
	state = NetworkClientState::Connected;
	onClientConnected(peer);
}

//-----------------------------------//

void NetworkClient::onDisconnected(const NetworkPeerPtr& peer)
{
	state = NetworkClientState::Disconnected;
	onClientDisconnected(peer);
}

//-----------------------------------//

void NetworkClient::onMessage(const NetworkPeerPtr& peer, const MessagePtr& message)
{
	onServerMessage(peer, message);
}

//-----------------------------------//

bool NetworkServer::createSocket( const String& address, int port )
{
	ENetAddress addr;
	addr.host = ENET_HOST_ANY;
	addr.port = port;

	host = CreateEnetSocket(&addr);
	
	return (host != nullptr);
}

//-----------------------------------//

void NetworkServer::onConnected(const NetworkPeerPtr& peer)
{
	peers.push_back(peer);
	onClientConnected(peer);
}

//-----------------------------------//

void NetworkServer::onDisconnected(const NetworkPeerPtr& peer)
{
	onClientDisconnected(peer);
	
	NetworkPeers::iterator it = std::find(peers.begin(), peers.end(), peer);
	assert( it != peers.end() );
	
	peers.erase(it);
}

//-----------------------------------//

void NetworkServer::onMessage(const NetworkPeerPtr& peer, const MessagePtr& message)
{
	onClientMessage(peer, message);
}

//-----------------------------------//

NAMESPACE_END