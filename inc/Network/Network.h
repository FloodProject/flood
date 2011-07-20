/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"
#include "Core/Concurrency.h"
#include "Core/Event.h"

struct _ENetHost;
typedef _ENetHost ENetHost;

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

struct _ENetEvent;
typedef _ENetEvent ENetEvent;

FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

API_CORE bool NetworkInitialize();
API_CORE void NetworkDeinitialize();

// Peers are representations of network entities.
class NetworkPeer : public ReferenceCounted
{
public:

	NetworkPeer();

	// Gets the peer hostname.
	String getHostName() const;

	// Gets the peer IP address.
	String getHostIP() const;

	// Queues a message to be sent to the peer.
	void queueMessage(const MessagePtr& message, uint8 channel);

	ENetPeer* peer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( NetworkPeer )
typedef std::vector<NetworkPeerPtr> NetworkPeers;

//-----------------------------------//

// Hosts can be either servers or clients.
class NetworkHost : public ReferenceCounted
{
public:

	// Checks the host for events.
	void processEvents(uint32 timeout);

protected:

	NetworkHost();
	~NetworkHost();

	// High-level events.
	virtual void onConnected(const NetworkPeerPtr&) {}
	virtual void onDisconnected(const NetworkPeerPtr&) {}
	virtual void onMessage(const NetworkPeerPtr&, const MessagePtr&) {}

	// Low-level events.
	void handleConnectEvent(ENetEvent* event);
	void handleDisconnectEvent(ENetEvent* event);
	void handleReceiveEvent(ENetEvent* event);

	// Host context.
	ENetHost* host;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( NetworkHost )

//-----------------------------------//

namespace NetworkClientState
{
	enum Enum
	{
		Initial,
		Connecting,
		Connected,
		Disconnecting,
		Disconnected
	};
}

// Clients connect to hosts.
class NetworkClient : public NetworkHost
{
public:

	NetworkClient();
	
	// Connects the client to a remote host.
	bool connect( const String& address, int port );

	// Gets the network peer.
	GETTER(Peer, const NetworkPeerPtr&, peer)

	// Gets the client state.
	GETTER(State, NetworkClientState::Enum, state)

	Event1<const NetworkPeerPtr&> onClientConnected;
	Event1<const NetworkPeerPtr&> onClientDisconnected; 
	Event2<const NetworkPeerPtr&, const MessagePtr&> onServerMessage;

protected:

	void onConnected(const NetworkPeerPtr&) OVERRIDE;
	void onDisconnected(const NetworkPeerPtr&) OVERRIDE;
	void onMessage(const NetworkPeerPtr&, const MessagePtr&) OVERRIDE;

	NetworkPeerPtr peer;
	NetworkClientState::Enum state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( NetworkClient )

//-----------------------------------//

// Hosts are servers that accepts clients and service peers.
class NetworkServer : public NetworkHost
{
public:

	// Creates a new socket.
	bool createSocket( const String& address, int port );

	Event1<const NetworkPeerPtr&> onClientConnected;
	Event1<const NetworkPeerPtr&> onClientDisconnected; 
	Event2<const NetworkPeerPtr&, const MessagePtr&> onClientMessage;

protected:

	void onConnected(const NetworkPeerPtr&) OVERRIDE;
	void onDisconnected(const NetworkPeerPtr&) OVERRIDE;
	void onMessage(const NetworkPeerPtr&, const MessagePtr&) OVERRIDE;	

	// Network peers.
	NetworkPeers peers;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( NetworkServer )

//-----------------------------------//

NAMESPACE_CORE_END