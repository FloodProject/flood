/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/Concurrency.h"
#include "Core/ConcurrentQueue.h"
#include "Core/Event.h"
#include "Core/Network/Peer.h"

typedef struct _ENetHost ENetHost;
typedef struct _ENetEvent ENetEvent;

FWD_DECL_INTRUSIVE(Packet)
FWD_DECL_INTRUSIVE(Peer)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// Hosts can be either servers or clients.
class API_CORE  Host : public ReferenceCounted
{
public:

	// Destroys the current socket.
	bool destroySocket();

	// Broadcasts a packet.
	void broadcastPacket(const PacketPtr&, uint8 channel = 0);

	// Checks the host for events.
	void processEvents(uint32 timeout);

	// Returns if the host has a networking context.
	bool hasContext();

protected:

	Host();
	~Host();

	// High-level events.
	virtual void onConnected(const PeerPtr&) {}
	virtual void onDisconnected(const PeerPtr&) {}
	virtual void onPacket(const PeerPtr&, const PacketPtr&) {}

	// Low-level events.
	void handleConnectEvent(ENetEvent* event);
	void handleDisconnectEvent(ENetEvent* event);
	void handleReceiveEvent(ENetEvent* event);

	// Host context.
	ENetHost* host;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Host )

//-----------------------------------//

enum class HostState
{
	Disconnected,
	Connecting,
	Connected,
	Disconnecting,
};

struct FLD_VALUE_TYPE HostConnectionDetails
{
	String address;
	uint16 port;
	uint8 channelCount;

	HostConnectionDetails()
		: address("")
		, port(0)
		, channelCount(1)
	{}

	HostConnectionDetails(String address, uint16 port, uint8 channelCount = 1)
		: address(address)
		, port(port)
		, channelCount(channelCount)
	{}
};

// Clients connect to hosts.
class API_CORE HostClient : public Host
{
public:

	HostClient();
	
	// Connects the client to a remote host.
	bool connect( const HostConnectionDetails& );

	// Gets the network peer.
	GETTER(Peer, const PeerPtr&, peer)

	// Gets the client state.
	ACCESSOR(State, HostState, state)

	Event1<const PeerPtr&> onClientConnected;
	Event1<const PeerPtr&> onClientDisconnected; 
	Event2<const PeerPtr&, const PacketPtr&> onServerPacket;

protected:

	void onConnected(const PeerPtr&) OVERRIDE;
	void onDisconnected(const PeerPtr&) OVERRIDE;
	void onPacket(const PeerPtr&, const PacketPtr&) OVERRIDE;

	PeerPtr peer;
	HostState state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostClient )

//-----------------------------------//

// Hosts are servers that accepts clients and service peers.
class API_CORE HostServer : public Host
{
public:

	// Creates a new socket.
	bool createSocket( const HostConnectionDetails& );

	// Gets the network peers.
	GETTER(Peers, const NetworkPeers&, peers)

	Event1<const PeerPtr&> onClientConnected;
	Event1<const PeerPtr&> onClientDisconnected; 
	Event2<const PeerPtr&, const PacketPtr&> onClientPacket;

protected:

	void onConnected(const PeerPtr&) OVERRIDE;
	void onDisconnected(const PeerPtr&) OVERRIDE;
	void onPacket(const PeerPtr&, const PacketPtr&) OVERRIDE;	

	// Network peers.
	NetworkPeers peers;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostServer )

//-----------------------------------//

NAMESPACE_CORE_END