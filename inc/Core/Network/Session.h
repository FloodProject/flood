/************************************************************************
*
* vapor3D Core © (2008-2010)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/Network/CipherISAAC.h"

FWD_DECL_INTRUSIVE(Peer)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Enumerates all the states a network session can be.
 */

enum class SessionState
{
	Connecting,
	Connected,
	Authenticated
};

//-----------------------------------//

/**
 * Sessions are higher-level than peers. They allow for example for
 * peers to reconnect to their game even if their connection drops.
 * Sessions will be usually terminated when a timeout expires or if
 * the peer explicitly terminates their connection.
 */

class Session : public ReferenceCounted
{
public:

	Session();
	~Session();

	// Accesses the peer associated with this session.
	ACESSOR(Peer, const PeerPtr&, peer)

	// Handles session authentication.
	void handleAuthentication();

	// Handles session connection.
	void handleConnection();

protected:

	SessionState state;
	CipherISAAC cipher;
	PeerPtr peer;

	float lastConnection;
	float lastCommunication;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Session);

//-----------------------------------//

/**
 * Session groups are used to keep together a related set of
 * sessions. You can use these to for example, group sessions
 * by teams to broadcast a set of messages to the group.
 */

class SessionGroup
{


};

//-----------------------------------//

NAMESPACE_CORE_END
