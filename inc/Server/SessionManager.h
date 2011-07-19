/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Server/Session.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

typedef std::vector<SessionPtr> Sessions;

class SessionManager
{
public:

	SessionManager();
	~SessionManager();

	// Adds a new session to the manager.
	void addSession(const SessionPtr& session);

protected:

	Sessions sessions;
};

//-----------------------------------//

NAMESPACE_SERVER_END
