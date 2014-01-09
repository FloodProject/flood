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
    assert(hash && sessions.Find(*hash) == sessions.End());

    sessions[*hash] = session;

    onSessionAdded(session);
}

//-----------------------------------//

void SessionManager::removeSession(const SessionPtr& session)
{
    if( !session ) return;

    SessionHash* hash = session->getHash();
    assert(hash);

    auto it = sessions.Find(*hash);
    
    if (it == sessions.End())
        return;

    sessions.Erase(it);

    onSessionRemoved(session);
}

//-----------------------------------//

Session* SessionManager::getSession(const SessionHash& hash) const
{
    auto it = sessions.Find(hash);

    if (it == sessions.End())
        return nullptr;

    return it->second.get();
}

NAMESPACE_CORE_END