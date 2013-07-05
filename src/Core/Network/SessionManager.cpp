/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/SessionManager.h"
#include "Core/Network/Peer.h"
#include "Core/Containers/Hash.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

SessionManager::SessionManager()
	: sessions(*AllocatorGetHeap())
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
	const PeerPtr& peer = session->getPeer();
	
	hash::set(sessions, (uint64)session.get(), session);

	onSessionAdded(session);
}

//-----------------------------------//

void SessionManager::removeSession(const SessionPtr& session)
{
	if( !session ) return;
	const PeerPtr& peer = session->getPeer();

	auto s = hash::get(sessions, (uint64)peer.get(), SessionPtr(nullptr));
	if(!s)
		return;

	hash::remove(sessions, (uint64)s.get());

	onSessionRemoved(session);
}

//-----------------------------------//

SessionPtr SessionManager::getSession(const PeerPtr& peer)
{
	return hash::get(sessions, (uint64)peer.get(), SessionPtr(nullptr));
}

//-----------------------------------//

NAMESPACE_CORE_END