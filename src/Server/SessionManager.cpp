/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/SessionManager.h"
#include "Network/Network.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

SessionManager::SessionManager()
{
}

//-----------------------------------//

SessionManager::~SessionManager()
{
}

//-----------------------------------//

void SessionManager::addSession(const SessionPtr& session)
{
	if( !session ) return;
	const NetworkPeerPtr& peer = session->getPeer();
	
	sessions[peer] = session;

	onSessionAdded(session);
}

//-----------------------------------//

void SessionManager::removeSession(const SessionPtr& session)
{
	if( !session ) return;
	const NetworkPeerPtr& peer = session->getPeer();

	SessionsMap::iterator it = sessions.find(peer);
	
	if( it == sessions.end() )
		return;

	sessions.erase(it);

	onSessionRemoved(session);
}

//-----------------------------------//

SessionPtr SessionManager::getSession(const NetworkPeerPtr& peer)
{
	SessionsMap::iterator it = sessions.find(peer);

	if( it == sessions.end() )
		return nullptr;

	return it->second;
}

//-----------------------------------//

NAMESPACE_SERVER_END