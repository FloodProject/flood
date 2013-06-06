/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/SessionManager.h"
#include "Core/Network/Peer.h"

NAMESPACE_CORE_BEGIN

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

	SessionHash* hash = session->getHash();
	assert(hash && sessions.find(*hash) == sessions.end());

	sessions[*hash] = session;

	onSessionAdded(session);
}

//-----------------------------------//

void SessionManager::removeSession(const SessionPtr& session)
{
	if( !session ) return;

	SessionHash* hash = session->getHash();
	assert(hash);

	auto it = sessions.find(*hash);
	
	if( it == sessions.end() )
		return;

	sessions.erase(it);

	onSessionRemoved(session);
}

//-----------------------------------//

Session* SessionManager::getSession(const SessionHash& hash) const
{
	auto it = sessions.find(hash);

	if( it == sessions.end() )
		return nullptr;

	return it->second.get();
}

NAMESPACE_CORE_END