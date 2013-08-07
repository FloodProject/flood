/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Session.h"
#include "SessionManager.h"
#include "Core/References.h"
#include "Core/Event.h"
#include "Core/String.h"

typedef struct _ENetHost ENetHost;
typedef struct _ENetEvent ENetEvent;

FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Packet)
FWD_DECL_INTRUSIVE(Session)

NAMESPACE_CORE_BEGIN

class PacketProcessors;

//-----------------------------------//

// Hosts can be either servers or clients.
class API_CORE  Host : public ReferenceCounted
{
public:

	~Host();

	// Destroys the current socket.
	bool destroySocket();

	// Broadcasts a packet.
	void broadcastPacket(const PacketPtr&, uint8 channel = 0);

	// Checks the host for events.
	void processEvents(uint32 timeout);

	// Returns if the host has a networking context.
	bool hasContext();

	Event1<const PeerPtr&> onPeerConnect;
	Event1<const PeerPtr&> onPeerDisconnect;
	Event3<const PeerPtr&, const PacketPtr&, int> onPeerPacket;

protected:

	Host();

	// High-level events.
	virtual void onPeerConnect(const PeerPtr&) {}
	virtual void onPeerDisconnect(const PeerPtr&) {}

	// Host context.
	ENetHost* host;

	PacketProcessors* processors;

private:

	// Low-level events.
	void handleConnectEvent(ENetEvent* event);
	void handleDisconnectEvent(ENetEvent* event);
	void handleReceiveEvent(ENetEvent* event);

};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Host )

//-----------------------------------//

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

//-----------------------------------//

// Clients connect to hosts.
class API_CORE HostClient : public Host
{
public:

	HostClient();
	
	// Connects the client to a remote host.
	bool connect( const HostConnectionDetails& );

	GETTER(Peer, PeerPtr, peer);
	GETTER(Session, Session*, session);

protected:

	void onPeerConnect(const PeerPtr&) OVERRIDE;

	PeerPtr peer;
	Session* session;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostClient )

//-----------------------------------//

// Hosts are servers that accepts clients and service peers.
class API_CORE HostServer : public Host
{
public:

	// Creates a new socket.
	bool createSocket( const HostConnectionDetails& );

	GETTER(Peers, const std::vector<PeerPtr>&, peers);
	SessionManager& getSessionManager() { return sessions; }

protected:

	void onPeerConnect(const PeerPtr&) OVERRIDE;
	void onPeerDisconnect(const PeerPtr&) OVERRIDE;

	std::vector<PeerPtr> peers;
	SessionManager sessions;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( HostServer )

//-----------------------------------//

NAMESPACE_CORE_END