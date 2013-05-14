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

FWD_DECL_INTRUSIVE(Message)
FWD_DECL_INTRUSIVE(Peer)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// Hosts can be either servers or clients.
class Host : public ReferenceCounted
{
public:

	// Destroys the current socket.
	bool destroySocket();

	// Broadcasts a message.
	void broadcastMessage(const MessagePtr&, uint8 channel = 0);

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
	virtual void onMessage(const PeerPtr&, const MessagePtr&) {}

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
};

// Clients connect to hosts.
class HostClient : public Host
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
	Event2<const PeerPtr&, const MessagePtr&> onServerMessage;

protected:

	void onConnected(const PeerPtr&) OVERRIDE;
	void onDisconnected(const PeerPtr&) OVERRIDE;
	void onMessage(const PeerPtr&, const MessagePtr&) OVERRIDE;

	PeerPtr peer;
	HostState state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostClient )

//-----------------------------------//

// Hosts are servers that accepts clients and service peers.
class HostServer : public Host
{
public:

	// Creates a new socket.
	bool createSocket( const HostConnectionDetails& );

	// Gets the network peers.
	GETTER(Peers, const NetworkPeers&, peers)

	Event1<const PeerPtr&> onClientConnected;
	Event1<const PeerPtr&> onClientDisconnected; 
	Event2<const PeerPtr&, const MessagePtr&> onClientMessage;

protected:

	void onConnected(const PeerPtr&) OVERRIDE;
	void onDisconnected(const PeerPtr&) OVERRIDE;
	void onMessage(const PeerPtr&, const MessagePtr&) OVERRIDE;	

	// Network peers.
	NetworkPeers peers;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostServer )

//-----------------------------------//

NAMESPACE_CORE_END