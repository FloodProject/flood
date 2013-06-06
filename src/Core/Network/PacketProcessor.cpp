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
    for (auto it = processors.rbegin(); it != processors.rend(); it++) 
    {
        if(!(*it)->processInPacket(peer, packet,channelId))
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

    processors.push_back(packetProcessor);
    packetProcessor->parent = this;
}

void PacketProcessors::addProcessorNear(PacketProcessor* packetProcessor, PacketProcessor* nearPacketProcessor, bool insertBefore)
{
    auto it = std::find(processors.begin(), processors.end(), nearPacketProcessor);
    assert( it != processors.end() );

    if(!insertBefore)
        it++;

    processors.insert(it, packetProcessor);
    packetProcessor->parent = this;
}

void PacketProcessors::removeProcessor(PacketProcessor* packetProcessor)
{
    auto it = std::find(processors.begin(), processors.end(), packetProcessor);
    if( it != processors.end() )
        processors.erase(it);
}

NAMESPACE_CORE_END