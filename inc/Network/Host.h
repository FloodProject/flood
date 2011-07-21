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
#include "Network/Peer.h"

struct _ENetHost;
typedef _ENetHost ENetHost;

struct _ENetEvent;
typedef _ENetEvent ENetEvent;

FWD_DECL_INTRUSIVE(Message)
FWD_DECL_INTRUSIVE(Peer)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// Hosts can be either servers or clients.
class Host : public ReferenceCounted
{
public:

	// Checks the host for events.
	void processEvents(uint32 timeout);

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
class HostClient : public Host
{
public:

	HostClient();
	
	// Connects the client to a remote host.
	bool connect( const String& address, int port );

	// Gets the network peer.
	GETTER(Peer, const PeerPtr&, peer)

	// Gets the client state.
	GETTER(State, NetworkClientState::Enum, state)

	Event1<const PeerPtr&> onClientConnected;
	Event1<const PeerPtr&> onClientDisconnected; 
	Event2<const PeerPtr&, const MessagePtr&> onServerMessage;

protected:

	void onConnected(const PeerPtr&) OVERRIDE;
	void onDisconnected(const PeerPtr&) OVERRIDE;
	void onMessage(const PeerPtr&, const MessagePtr&) OVERRIDE;

	PeerPtr peer;
	NetworkClientState::Enum state;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostClient )

//-----------------------------------//

// Hosts are servers that accepts clients and service peers.
class HostServer : public Host
{
public:

	// Creates a new socket.
	bool createSocket( const String& address, int port );

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