/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Processors/PacketKeyExchanger.h"
#include "Core/Network/Processors/PacketEncryptor.h"
#include "Core/Network/Packet.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Host.h"
#include "Core/Network/Network.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

#include <tropicssl/dhm.h>
#include <tropicssl/sha1.h>
#include <tropicssl/havege.h>

#define DHM_RFC5114_MODP_1024_P                            \
        "B10B8F96A080E01DDE92DE5EAE5D54EC52C99FBCFB06A3C6" \
        "9A6A9DCA52D23B616073E28675A23D189838EF1E2EE652C0" \
        "13ECB4AEA906112324975C3CD49B83BFACCBDD7D90C4BD70" \
        "98488E9C219A73724EFFD6FAE5644738FAA31A4FF55BCCC0" \
        "A151AF5F0DC8B4BD45BF37DF365C1A65E68CFDA76D4DA708" \
        "DF1FB2BC2E4A4371"

#define DHM_RFC5114_MODP_1024_G                            \
        "A4D1CBD5C3FD34126765A442EFB99905F8104DD258AC507F" \
        "D6406CFF14266D31266FEA1E5C41564B777E690F5504F213" \
        "160217B4B01B886A5E91547F9E2749F4D7FBD7D3B9A92EE1" \
        "909D0D2263F80A76A6A24C087A091F531DBF0A0169B6A28A" \
        "D662A4D18E73AFA32D779D5918D08BC8858F4DCEF97C2A24" \
        "855E6EEB22B3B2E5"

//Size of DHM's secret in bits
const int DHM_SECRET_SIZE = 256; 

NAMESPACE_CORE_BEGIN

//-----------------------------------//

enum class KeyExchangePacket : uint16
{
    ClientPublicKey = 1,
    ServerPublicKey,
    ClientSession,
    ServerSessionAck
};

//-----------------------------------//

static void InitSessionHash( SessionHash& hash, const std::vector<uint8>& secret )
{
    sha1((unsigned char*)secret.data(), secret.size(), hash.data());
}

//-----------------------------------//

PacketKeyExchanger::PacketKeyExchanger()
{
    havege_init(&hs);

    memset(&dhm, 0, sizeof(dhm));
    mpi_init(&dhm.P, &dhm.G, nullptr);
    mpi_read_string( &dhm.P, 16, DHM_RFC5114_MODP_1024_P );
    mpi_read_string( &dhm.G, 16, DHM_RFC5114_MODP_1024_G );
    dhm.len = mpi_size(&dhm.P);
}

PacketKeyExchanger::~PacketKeyExchanger()
{
    dhm_free(&dhm);
}

//-----------------------------------//

PacketClientKeyExchanger::PacketClientKeyExchanger()
    : state(ClientKeyExchangerState::WaitingServerPublicKey)
{
}

bool PacketClientKeyExchanger::processInPacket(Peer* peer, Packet* packet, int channelId)
{
    switch(state)
    {
    case ClientKeyExchangerState::WaitingServerPublicKey:
        if (processServerPublicKeyPacket(peer, packet, channelId))
        {
            state = ClientKeyExchangerState::WaitingServerSession;
            return false;
        }
        break;
    case ClientKeyExchangerState::WaitingServerSession:
        if (processServerSessionAckPacket(peer, packet, channelId))
        {
            peer->getSession()->setState(SessionState::Open);
            parent->removeProcessor(this);
            return false;
        }
        break;
    }

    LogError("Unsuccessful key exchange, disconecting peer.");
    peer->forceDisconnect();
    return false;
}

void PacketClientKeyExchanger::beginKeyExchange(Peer* peer)
{
    sendClientPublicKeyPacket(peer, /*channelId=*/0);
}

void PacketClientKeyExchanger::sendClientPublicKeyPacket(Peer* peer, int channelId)
{
    uint8 buf[1024];

    if (int ret = dhm_make_public(&dhm, DHM_SECRET_SIZE, buf, dhm.len, havege_rand, &hs))
    {
        LogError("dhm_make_public returned %d", ret);
        return;
    }

    PacketPtr packet = PacketCreate((PacketId)KeyExchangePacket::ClientPublicKey);
    packet->write(buf, dhm.len);
    peer->queuePacket(packet, channelId);
}

bool PacketClientKeyExchanger::processServerPublicKeyPacket(Peer* peer, Packet* packet, int channelId)
{
    if ( packet->getId() != (PacketId)KeyExchangePacket::ServerPublicKey )
        return false;

    // Read server's DH parameters
    auto buf = packet->read();
    uint8 *p = buf.data();
    if (int ret = dhm_read_params(&dhm, &p, p + buf.size())) 
    {
        LogError("dhm_read_params returned %d", ret);
        return false;
    }

    // Derive the shared secret
    std::vector<uint8> secret(dhm.len);
    if (int ret = dhm_calc_secret(&dhm, secret.data(), &dhm.len)) 
    {
        LogError("dhm_calc_secret returned %d", ret);
        return false;
    }

    auto encryptor = Allocate(AllocatorGetNetwork(), PacketEncryptor, secret);
    parent->addProcessor(encryptor);

    Session* session = peer->getSession();
    SessionHash* hash = session->getHash();

    sendClientSessionPacket(peer, channelId, hash);

    if (!hash)
    {
        SessionHash newHash;
        InitSessionHash(newHash, secret);
        session->setHash(newHash);
    }

    return true;
}

void PacketClientKeyExchanger::sendClientSessionPacket(Peer* peer, int channelId, SessionHash* hash)
{
    PacketPtr packet = PacketCreate((PacketId)KeyExchangePacket::ClientSession);

    if (hash)
        packet->write(hash->data(), hash->size());

    packet->setFlags(PacketFlags::Encrypted);
    peer->queuePacket(packet, channelId);
}

bool PacketClientKeyExchanger::processServerSessionAckPacket(Peer* peer, Packet* packet, int channelId)
{
    if (packet->getId() != (PacketId)KeyExchangePacket::ServerSessionAck)
        return false;

    std::vector<uint8> data = packet->read();
    if (data.size() != sizeof(ServerSessionStatus))
    {
        LogError("Unexpected server message size.");
        return false;
    }

    if ((data.data())[0] == (uint8) ServerSessionStatus::Accepted)
        return true;

    return false;
}

//-----------------------------------//

PacketServerKeyExchanger::PacketServerKeyExchanger()
     : state(ServerKeyExchangerState::WaitingClientPublicKey)
{
}

bool PacketServerKeyExchanger::processInPacket(Peer* peer, Packet* packet, int channelId)
{
    switch(state)
    {
    case ServerKeyExchangerState::WaitingClientPublicKey:
        if (processClientPublicKeyPacket(peer, packet, channelId))
        {
            state = ServerKeyExchangerState::WaitingClientSession;
            return false;
        }
        break;
    case ServerKeyExchangerState::WaitingClientSession:
        if (processClientSessionPacket(peer, packet, channelId))
        {
            peer->getSession()->setState(SessionState::Open);
            parent->removeProcessor(this);
            return false;
        }
        break;
    }

    LogError("Unsuccessful key exchange, disconnecting peer.");
    peer->forceDisconnect();
    return false;
}

bool PacketServerKeyExchanger::processClientPublicKeyPacket(Peer* peer, Packet* packet, int channelId)
{
    if ( packet->getId() != (PacketId)KeyExchangePacket::ClientPublicKey )
        return false;

    // Receive client's public value
    if (int ret = dhm_read_public(&dhm, packet->read().data(), dhm.len)) 
    {
        LogError("dhm_read_public returned %d", ret);
        return false;
    }

    // Setup the DH parameters (P,G,Ys)
    if (int ret = dhm_make_params(&dhm, 256, publicKey, &publicKeySize, havege_rand, &hs)) 
    {
        LogError("dhm_make_params returned %d", ret);
        return false;
    }

    // Derive the shared secret
    std::vector<uint8> secret(dhm.len);
    if (int ret = dhm_calc_secret(&dhm, secret.data(), &dhm.len)) 
    {
        LogError("dhm_calc_secret returned %d", ret);
        return false;
    }

    auto encryptor = Allocate(AllocatorGetNetwork(), PacketEncryptor, secret);
    parent->addProcessor(encryptor);

    InitSessionHash(newHash, secret);

    // Send the DH parameters
    if (!sendServerPublicKeyPacket(peer, channelId))
        return false;

    return true;
}

bool PacketServerKeyExchanger::sendServerPublicKeyPacket(Peer* peer, int channelId)
{
    // Create a packet with the parameters and signature
    PacketPtr packet = PacketCreate((PacketId) KeyExchangePacket::ServerPublicKey);
    packet->setFlags(PacketFlags::Signed);
    packet->write(publicKey, publicKeySize);

    peer->queuePacket(packet, channelId);

    return true;
}

bool PacketServerKeyExchanger::processClientSessionPacket(Peer* peer, Packet* packet, int channelId)
{
    if ( packet->getId() != (PacketId)KeyExchangePacket::ClientSession)
        return false;

    HostServer* server = (HostServer*) peer->getHost();
    std::vector<uint8> &data = packet->read();
    Session* session;

    if (data.size() != 0)
    {
        SessionHash oldHash;

        if (oldHash.size() != data.size())
            return false;

        memcpy(oldHash.data(), data.data(), data.size());

        session = server->getSessionManager().getSession(oldHash);

        if (!session)
        {
            sendServerSessionAckPacket(peer, channelId, ServerSessionStatus::Refused);
            return false;
        }
    }
    else
    {
        session = Allocate(AllocatorGetNetwork(), Session);
        session->setHash(newHash);
        server->getSessionManager().addSession(session);
    }

    peer->setSession(session);
    session->setPeer(peer);

    sendServerSessionAckPacket(peer, channelId, ServerSessionStatus::Accepted);

    return true;
}

void PacketServerKeyExchanger::sendServerSessionAckPacket(Peer* peer, int channelId, ServerSessionStatus status)
{
    PacketPtr packet = PacketCreate((PacketId)KeyExchangePacket::ServerSessionAck);
    packet->write((uint8*)&status, sizeof(ServerSessionStatus));
    peer->queuePacket(packet, channelId);
}

NAMESPACE_CORE_END