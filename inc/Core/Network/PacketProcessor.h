/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/References.h"
#include "Core/Containers/Vector.h"

FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Packet)
FWD_DECL_INTRUSIVE(PacketProcessors)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class PacketProcessor : public ReferenceCounted
{
public:

    PacketProcessor() : parent(nullptr) {}
    virtual ~PacketProcessor() {}

    virtual bool processInPacket(Peer* peer, Packet* packet, int channelId)
    { 
        return true; 
    }

    virtual bool processOutPacket(Peer* peer, Packet* packet, int channelId)
    { 
        return true; 
    }

    PacketProcessors* parent;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( PacketProcessor )

//-----------------------------------//

class PacketProcessors : public PacketProcessor
{
public:

    PacketProcessors();
    ~PacketProcessors();

    bool processInPacket(Peer* peer, Packet* packet, int channelId) OVERRIDE;
    bool processOutPacket(Peer* peer, Packet* packet, int channelId) OVERRIDE;

    void addProcessor(PacketProcessor* packetProcessor);
    void addProcessorNear(PacketProcessor* packetProcessor, PacketProcessor* nearPacketProcessor, bool insertBefore);
    void removeProcessor(PacketProcessor* packetProcessor);

private:

    Vector<PacketProcessor*> processors;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( PacketProcessors )

//-----------------------------------//

NAMESPACE_CORE_END