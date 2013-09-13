/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/PacketProcessor.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Packet.h"
#include "Core/Network/Network.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

#include <tropicssl/aes.h>
#include <tropicssl/sha1.h>
#include <tropicssl/rsa.h>

NAMESPACE_CORE_BEGIN

PacketProcessors::PacketProcessors()
{}

PacketProcessors::~PacketProcessors()
{
}

bool PacketProcessors::processInPacket(Peer* peer, Packet* packet, int channelId)
{
    for (auto i = processors.size(); i > 0; --i)
    {
        auto p = processors[i - 1];
        if(!p->processInPacket(peer, packet,channelId))
            return false;
    }
    return true;
}

bool PacketProcessors::processOutPacket(Peer* peer, Packet* packet, int channelId)
{
    for (auto it = processors.begin(); it != processors.end(); it++) 
    {
        if(!(*it)->processOutPacket(peer, packet,channelId))
            return false;
    }
    return true;
}

void PacketProcessors::addProcessor(PacketProcessor* packetProcessor)
{
    if(packetProcessor->parent)
        packetProcessor->parent->removeProcessor(packetProcessor);

    processors.pushBack(packetProcessor);
    packetProcessor->parent = this;
}

void PacketProcessors::addProcessorNear(PacketProcessor* packetProcessor, PacketProcessor* nearPacketProcessor, bool insertBefore)
{
    auto it = std::find(processors.begin(), processors.end(), nearPacketProcessor);
    assert( it != processors.end() );

    if(!insertBefore)
        it++;

    processors.pushBack(packetProcessor);
    for(size_t i = &processors.back() - it; i > 0; --i)
        std::swap(processors[i], processors[i - 1]);
    packetProcessor->parent = this;
}

void PacketProcessors::removeProcessor(PacketProcessor* packetProcessor)
{
    auto it = std::find(processors.begin(), processors.end(), packetProcessor);
    if( it != processors.end() )
        processors.remove(it);
}

NAMESPACE_CORE_END