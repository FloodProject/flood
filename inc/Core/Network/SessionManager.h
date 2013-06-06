/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Event.h"
#include "Core/Network/Session.h"
#include <map>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class API_CORE SessionManager
{
public:

	SessionManager();
	~SessionManager();

	// Adds a session to the manager.
	FLD_IGNORE void addSession(const SessionPtr& session);

	// Removes a session from the manager.
	FLD_IGNORE void removeSession(const SessionPtr& session);

	// Gets a session with a given hash.
	Session* SessionManager::getSession(const SessionHash& hash) const;

	// Sent when a session is added.
	Event1<const SessionPtr&> onSessionAdded;

	// Sent when a session is removed.
	Event1<const SessionPtr&> onSessionRemoved;

protected:
	 std::map<SessionHash, SessionPtr> sessions;
};

//-----------------------------------//

NAMESPACE_CORE_END
