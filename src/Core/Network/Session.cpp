/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Session.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Host.h"
#include "Core/Network/Packet.h"
#include "Core/Network/Network.h"
#include "Core/Network/PacketProcessor.h"
#include "Core/Log.h"
#include "Core/Utilities.h"


NAMESPACE_CORE_BEGIN

//-----------------------------------//

Session::Session()
 : state(SessionState::Closed)
 , peer(nullptr)
 , hasHash(false)
{
}

//-----------------------------------//

Session::~Session()
{
}

//-----------------------------------//

void Session::setState(SessionState state)
{
    if(state == this->state)
        return;

    if(state == SessionState::Closed)
        peer = nullptr;

    assert(state == SessionState::Open && peer != nullptr && hasHash );

    this->state = state;
    onStateChange(state);
}

//-----------------------------------//

SessionHash* Session::getHash()
{
    if(!hasHash)
        return nullptr;

    return &hash;
}

//-----------------------------------//

void Session::setHash(const SessionHash& newHash)
{
    assert(!hasHash);
    hash = newHash;
    hasHash = true;
}

//-----------------------------------//

int Session::hashCode()
{
    return (int)getHash();
}

//-----------------------------------//

bool Session::isEquals(Session* session)
{
    return getHash() == session->getHash();
}

//-----------------------------------//

NAMESPACE_CORE_END