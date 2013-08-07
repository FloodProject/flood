/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/References.h"

#include <array>

FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Packet)
FWD_DECL_INTRUSIVE(Session)

NAMESPACE_CORE_BEGIN

FLD_IGNORE typedef std::array<uint8,20> SessionHash;

//-----------------------------------//

/**
 * Enumerates all the states a network session can be.
 */

enum class SessionState
{
	Closed,
	Open,
};

//-----------------------------------//

/**
 * Sessions are higher-level than peers. They allow for example for
 * peers to reconnect to their game even if their connection drops.
 * Sessions will be usually terminated when a timeout expires or if
 * the peer explicitly terminates their connection.
 */

class API_CORE Session : public ReferenceCounted
{
public:

	Session();
	~Session();

	GETTER(State, SessionState, state);
	FLD_IGNORE void setState(SessionState state);

	GETTER(Peer, Peer*, peer);
	FLD_IGNORE SETTER(Peer, Peer*, peer);

	SessionHash* getHash();
	FLD_IGNORE void setHash(const SessionHash& newHash);

	Event1<SessionState> onStateChange;
	Event2<const PacketPtr&, int> onPacket;

	FLD_HASHCODE int hashCode();
	FLD_EQUALS bool isEquals(Session* session);

private:

	SessionState state;
	Peer* peer;

	SessionHash hash;
	bool hasHash;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Session);

//-----------------------------------//

NAMESPACE_CORE_END
