/************************************************************************
*
* vapor3D Core © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"
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
	PeerPtr peer;
	CipherISAAC cipher;

	float lastConnection;
	float lastCommunication;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Session);

//-----------------------------------//

NAMESPACE_CORE_END
