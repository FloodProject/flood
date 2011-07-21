/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// Peers are representations of network entities.
class Peer : public ReferenceCounted
{
public:

	Peer();

	// Gets the peer hostname.
	String getHostName() const;

	// Gets the peer IP address.
	String getHostIP() const;

	// Queues a message to be sent to the peer.
	void queueMessage(const MessagePtr& message, uint8 channel);

	ENetPeer* peer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Peer )
typedef std::vector<PeerPtr> NetworkPeers;

//-----------------------------------//

NAMESPACE_CORE_END