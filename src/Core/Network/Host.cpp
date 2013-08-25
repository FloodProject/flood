/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Host.h"
#include "Core/Network/Network.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Session.h"
#include "Core/Network/Packet.h"
#include "Core/Network/PacketProcessor.h"
#include "Core/Network/Processors/PacketKeyExchanger.h"
#include "Core/Log.h"

#if defined(PLATFORM_WINDOWS) && !defined(WIN32)
#define WIN32
#endif

#include <enet/enet.h>

NAMESPACE_CORE_BEGIN

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

void Host::broadcastPacket(const PacketPtr& packet, uint8 channel)
{
	packet->prepare();
	enet_host_broadcast(host, channel, packet->getPacket());
}

//-----------------------------------//

void Host::handleConnectEvent(ENetEvent* event)
{
	ENetPeer* peer = event->peer;

	PeerPtr networkPeer = AllocateThis(Peer);
	networkPeer->setPeer(peer);
	networkPeer->setHost(this);

	// Store the network peer as user data.
	peer->data = networkPeer.get();

	networkPeer->setState(PeerState::Connected);
	onPeerConnect(networkPeer);
}

//-----------------------------------//

void Host::handleDisconnectEvent(ENetEvent* event)
{
	PeerPtr networkPeer = (Peer*) event->peer->data;
	if( !networkPeer ) return;

	networkPeer->setState(PeerState::Disconnected);
	onPeerDisconnect(networkPeer);

	// Reset the peer userdata.
	ENetPeer* peer = event->peer;
	peer->data = nullptr;
}

//-----------------------------------//

void Host::handleReceiveEvent(ENetEvent* event)
{
	ENetPacket* packet = event->packet;
	Peer* peer = (Peer*) event->peer->data;

	PacketPtr packetPtr = PacketCreate(0);
	packetPtr->setPacket(packet);

	if(peer->processInPacket(packetPtr.get(), event->channelID))
		if(peer->getSession())
			onSessionPacket(peer->getSession(), packetPtr, event->channelID);
}

//-----------------------------------//

HostClient::HostClient()
{

}

//-----------------------------------//

bool HostClient::connect( const HostConnectionDetails& details )
{
	if( peer && peer->getState() != PeerState::Disconnected )
		return false;

	host = CreateEnetSocket(nullptr);

	if( !host )
		return false;

	ENetAddress addr;
	addr.host = 0;
	addr.port = details.port;

	auto ret = enet_address_set_host( &addr, details.address.c_str() );
	if(ret < 0)
	{
		LogError("Cannot resolve host address ", details.address.c_str()); 
		return false;
	}

	enet_uint32 data = 0;

	ENetPeer* newPeer = enet_host_connect(host, &addr, details.channelCount, data);
	if (!newPeer)
	{
		LogError("No available peers for initiating an ENet connection.");
		return false;
	}

	return true;
}

//-----------------------------------//

void HostClient::onPeerConnect(const PeerPtr& newPeer)
{
	peer = newPeer;
	peer->setSession(&session);
	session.setPeer(peer.get());


	auto keyExchanger = Allocate(AllocatorGetNetwork(), PacketClientKeyExchanger);
	peer->addProcessor(keyExchanger);
	keyExchanger->beginKeyExchange(peer.get());
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

void HostServer::onPeerConnect(const PeerPtr& peer)
{
	peers.pushBack(peer);

	auto keyExchanger = Allocate(AllocatorGetNetwork(), PacketServerKeyExchanger);
	peer->addProcessor(keyExchanger);
}

//-----------------------------------//

void HostServer::onPeerDisconnect(const PeerPtr& peer)
{
	auto it = std::find(peers.begin(), peers.end(), peer);
	assert( it != peers.end() );
	
	peers.remove(it);
}

//-----------------------------------//

NAMESPACE_CORE_END