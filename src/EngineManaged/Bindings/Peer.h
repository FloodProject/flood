/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Peer.h>

namespace Flood
{
    ref class _ENetPeer;
    ref class Host;
    ref class Packet;
    ref class PacketProcessor;
    ref class PacketProcessors;
    ref class Peer;
    ref class Session;

    public ref class Peer : ICppInstance
    {
    public:
        property ::Peer* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Peer(::Peer* native);
        Peer(System::IntPtr native);
        Peer();
        property System::String^ HostName
        {
            System::String^ get();
        }
        property System::String^ HostIP
        {
            System::String^ get();
        }
        property Flood::Session^ Session
        {
            Flood::Session^ get();
        }
        void Disconnect();
        void ForceDisconnect();
        void QueuePacket(Flood::Packet^ packet, unsigned char channel);
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
