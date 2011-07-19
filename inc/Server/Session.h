/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"
#include "Network/Network.h"
#include "Server/CipherISAAC.h"

NAMESPACE_SERVER_BEGIN

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

	// Sets the peer associated with this session.
	SETTER(Peer, const NetworkPeerPtr&, peer)

protected:

	SessionState::Enum state;
	NetworkPeerPtr peer;
	CipherISAAC cipher;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Session);

//-----------------------------------//

NAMESPACE_SERVER_END
