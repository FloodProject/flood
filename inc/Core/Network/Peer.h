/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/String.h"

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

FWD_DECL_INTRUSIVE(Packet)

NAMESPACE_CORE_BEGIN

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
	void queuePacket(const PacketPtr &packet, uint8 channel);

	ENetPeer* peer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Peer )
typedef std::vector<PeerPtr> NetworkPeers;

//-----------------------------------//

NAMESPACE_CORE_END