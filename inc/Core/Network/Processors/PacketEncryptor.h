/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Network/PacketProcessor.h"
#include "Core/References.h"

NAMESPACE_CORE_BEGIN

typedef struct _aes_context aes_context;

//-----------------------------------//

class PacketEncryptor : public PacketProcessor
{
public:

    PacketEncryptor(std::vector<uint8> secret);
    ~PacketEncryptor();

    bool processInPacket(Peer* peer, Packet* packet, int channelId) OVERRIDE;
    bool processOutPacket(Peer* peer, Packet* packet, int channelId) OVERRIDE;

private:

    aes_context* aes;
    std::vector<uint8> secret;
    uint8 iv [16];

};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( PacketEncryptor )

//-----------------------------------//

NAMESPACE_CORE_END