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

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef std::map<PeerPtr, SessionPtr> SessionsMap;

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
	SessionPtr getSession(const PeerPtr& peer);

	// Sent when a session is added.
	Event1<const SessionPtr&> onSessionAdded;

	// Sent when a session is removed.
	Event1<const SessionPtr&> onSessionRemoved;

protected:

	SessionsMap sessions;
};

//-----------------------------------//

NAMESPACE_CORE_END
