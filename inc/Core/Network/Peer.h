/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Network/Session.h"
#include "Core/References.h"
#include "Core/String.h"
#include "Core/Event.h"

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

FWD_DECL_INTRUSIVE(Host)
FWD_DECL_INTRUSIVE(Packet)

NAMESPACE_CORE_BEGIN

class PacketProcessor;
class PacketProcessors;

//-----------------------------------//

enum class PeerState
{
	Disconnected,
	Connecting,
	Connected,
	Disconnecting,
};

//-----------------------------------//

// Peers are representations of network entities.
 class API_CORE Peer : public ReferenceCounted
{
public:

	Peer();

	// Asks for a normal disconnect.
	void disconnect();

	// Forces a disconnect.
	void forceDisconnect();

	// Gets the peer hostname.
	String getHostName() const;

	// Gets the peer IP address.
	String getHostIP() const;

	// Queues a packet to be sent to the peer.
	void queuePacket(const PacketPtr& packet, uint8 channel);

	FLD_IGNORE bool processInPacket(const PacketPtr& packet, uint8 channel);

	FLD_IGNORE void addProcessor(PacketProcessor* processor);

	FLD_IGNORE ACCESSOR(Peer, ENetPeer*, peer)

	FLD_IGNORE ACCESSOR(Host, Host*, host)

	GETTER(State, PeerState, state);
	FLD_IGNORE void setState(PeerState state);
	Event1<PeerState> onStateChanged;

	GETTER(Session, Session*, session)
	FLD_IGNORE SETTER(Session, Session*, session)

private:

	ENetPeer* peer;
	Host* host;
	Session* session;

	PeerState state;

	PacketProcessors* processors;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Peer )

//-----------------------------------//

NAMESPACE_CORE_END