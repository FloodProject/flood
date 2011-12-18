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
#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Message.h"

#include <enet/enet.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Allocator* gs_NetworkAllocator = nullptr;

Allocator* AllocatorGetNetwork()
{
	return gs_NetworkAllocator;
}

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

Peer::Peer()
	: peer(nullptr)
{
}

//-----------------------------------//

void Peer::queueMessage(const MessagePtr& message, uint8 channel)
{
	if( !message ) return;
	message->prepare();
	
	int status = enet_peer_send(peer, channel, message->getPacket());

	if(status  != 0)
	{
		LogWarn("Error sending packet to peer '%s'", getHostName().c_str());
		return;
	}
}

//-----------------------------------//

void Peer::disconnect()
{
	//enet_peer_disconnect_now(peer, 0);
	enet_peer_disconnect_later(peer, 0);
}

//-----------------------------------//

void Peer::forceDisconnect()
{
	enet_peer_reset(peer);
}

//-----------------------------------//

String Peer::getHostName() const
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

String Peer::getHostIP() const
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

Host::Host()
	: host(nullptr)
{
}

//-----------------------------------//

Host::~Host()
{
	destroySocket();
}

//-----------------------------------//

bool Host::destroySocket()
{
	if( !host ) return false;

	enet_host_destroy(host);
	host = nullptr;

	return true;
}

//-----------------------------------//

bool Host::hasContext()
{
	return host != nullptr;
}

//-----------------------------------//

void Host::processEvents(uint32 timeout)
{
	if( !hasContext() ) return;

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

void Host::broadcastMessage(const MessagePtr& message, uint8 channel)
{
	message->prepare();
	enet_host_broadcast(host, channel, message->getPacket());
}

//-----------------------------------//

void Host::handleConnectEvent(ENetEvent* event)
{
	ENetPeer* peer = event->peer;

	PeerPtr networkPeer = AllocateThis(Peer);
	networkPeer->peer = peer;

	// Store the network peer as user data.
	peer->data = networkPeer.get();

	onConnected(networkPeer);
}

//-----------------------------------//

void Host::handleDisconnectEvent(ENetEvent* event)
{
	PeerPtr networkPeer = (Peer*) event->peer->data;
	if( !networkPeer ) return;
	
	onDisconnected(networkPeer);

	// Reset the peer userdata.
	ENetPeer* peer = event->peer;
	peer->data = nullptr;
}

//-----------------------------------//

void Host::handleReceiveEvent(ENetEvent* event)
{
	ENetPacket* packet = event->packet;
	PeerPtr peer = (Peer*) event->peer->data;

	MessagePtr message = MessageCreate(0);
	message->setPacket(packet);

	onMessage(peer, message);
}

//-----------------------------------//

HostClient::HostClient()
	: state(HostState::Disconnected)
{
}

//-----------------------------------//

bool HostClient::connect( const HostConnectionDetails& details )
{
	if( state != HostState::Disconnected )
		return false;

	state = HostState::Connecting;

	host = CreateEnetSocket(nullptr);

	if( !host )
		return false;

	ENetAddress addr;
	addr.host = 0;
	addr.port = details.port;

	enet_address_set_host( &addr, details.address.c_str() );

	size_t channelCount = 2;
	enet_uint32 data = 0;

	ENetPeer* newPeer = enet_host_connect(host, &addr, channelCount, data);

	peer = Allocate(gs_NetworkAllocator, Peer);
	peer->peer = newPeer;

	return true;
}

//-----------------------------------//

void HostClient::onConnected(const PeerPtr& newPeer)
{
	peer = newPeer;
	state = HostState::Connected;
	onClientConnected(peer);
}

//-----------------------------------//

void HostClient::onDisconnected(const PeerPtr& peer)
{
	state = HostState::Disconnected;
	onClientDisconnected(peer);
}

//-----------------------------------//

void HostClient::onMessage(const PeerPtr& peer, const MessagePtr& message)
{
	onServerMessage(peer, message);
}

//-----------------------------------//

bool HostServer::createSocket( const HostConnectionDetails& details )
{
	ENetAddress addr;
	addr.host = ENET_HOST_ANY;
	addr.port = details.port;

	host = CreateEnetSocket(&addr);
	
	return (host != nullptr);
}

//-----------------------------------//

void HostServer::onConnected(const PeerPtr& peer)
{
	peers.push_back(peer);
	onClientConnected(peer);
}

//-----------------------------------//

void HostServer::onDisconnected(const PeerPtr& peer)
{
	onClientDisconnected(peer);
	
	NetworkPeers::iterator it = std::find(peers.begin(), peers.end(), peer);
	assert( it != peers.end() );
	
	peers.erase(it);
}

//-----------------------------------//

void HostServer::onMessage(const PeerPtr& peer, const MessagePtr& message)
{
	onClientMessage(peer, message);
}

//-----------------------------------//

NAMESPACE_CORE_END