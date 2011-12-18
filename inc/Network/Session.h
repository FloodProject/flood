/************************************************************************
*
* vapor3D Core © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Network/CipherISAAC.h"

FWD_DECL_INTRUSIVE(Peer)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

namespace SessionState
{
	enum Enum
	{
		Connecting,
		Connected,
		Authenticated,
	};
}

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

	SessionState::Enum state;
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
