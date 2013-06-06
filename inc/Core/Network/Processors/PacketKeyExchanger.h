/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Network/PacketProcessor.h"
#include "Core/Network/Session.h"
#include "Core/References.h"

#include <tropicssl/dhm.h>
#include <tropicssl/havege.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

enum class ServerSessionStatus : uint8
{
    Refused = 0,
    Accepted
};

//-----------------------------------//

class PacketKeyExchanger : public PacketProcessor
{
protected:

    PacketKeyExchanger();
    ~PacketKeyExchanger();

    dhm_context dhm;
    havege_state hs;
};

//-----------------------------------//

enum class ClientKeyExchangerState : uint8
{
    WaitingServerPublicKey,
    WaitingServerSession
};

class PacketClientKeyExchanger : public PacketKeyExchanger
{
public:

    PacketClientKeyExchanger();

    bool processInPacket(Peer* peer, Packet* packet, int channelId) OVERRIDE;

    void beginKeyExchange(Peer* peer);
    void beginKeyExchange(Peer* peer, SessionHash oldPeerHash);

private:

    void sendClientPublicKeyPacket(Peer* peer, int channelId);
    bool processServerPublicKeyPacket(Peer* peer, Packet* packet, int channelId);
    void sendClientSessionPacket(Peer* peer, int channelId, SessionHash* hash);
    bool processServerSessionAckPacket(Peer* peer, Packet* packet, int channelId);

    ClientKeyExchangerState state;
};

//-----------------------------------//

enum class ServerKeyExchangerState : uint8
{
    WaitingClientPublicKey,
    WaitingClientSession
};

class PacketServerKeyExchanger : public PacketKeyExchanger
{
public:

    PacketServerKeyExchanger();

    bool processInPacket(Peer* peer, Packet* packet, int channelId) OVERRIDE;

    void beginKeyExchange(Peer* peer);

private:

    bool processClientPublicKeyPacket(Peer* peer, Packet* packet, int channelId);
    bool sendServerPublicKeyPacket(Peer* peer, int channelId);
    bool processClientSessionPacket(Peer* peer, Packet* packet, int channelId);
    void sendServerSessionAckPacket(Peer* peer, int channelId, ServerSessionStatus status);

    ServerKeyExchangerState state;

    uint8 publicKey[1024];
    int publicKeySize;

    SessionHash newHash;
};

//-----------------------------------//

NAMESPACE_CORE_END