/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Event.h"
#include "Network/Session.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

typedef std::map<NetworkPeerPtr, SessionPtr> SessionsMap;

class SessionManager
{
public:

	SessionManager();
	~SessionManager();

	// Adds a session to the manager.
	void addSession(const SessionPtr& session);

	// Removes a session from the manager.
	void removeSession(const SessionPtr& session);

	// Gets a session from a network peer.
	SessionPtr getSession(const NetworkPeerPtr& peer);

	// Sent when a session is added.
	Event1<const SessionPtr&> onSessionAdded;

	// Sent when a session is removed.
	Event1<const SessionPtr&> onSessionRemoved;

protected:

	SessionsMap sessions;
};

//-----------------------------------//

NAMESPACE_SERVER_END
