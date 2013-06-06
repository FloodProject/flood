/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Network/Peer.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class _ENetPeer;
    ref class Host;
    ref class Packet;
    ref class PacketProcessor;
    ref class PacketProcessors;
    ref class Peer;
    ref class Session;

    public ref class Peer
    {
    public:
        property ::Peer* NativePtr;

        Peer(::Peer* native);
        Peer(System::IntPtr native);
        Peer();
        void Disconnect();
        void ForceDisconnect();
        System::String^ GetHostName();
        System::String^ GetHostIP();
        void QueuePacket(Flood::Packet^ packet, unsigned char channel);
        Flood::Session^ GetSession();
    };
}
