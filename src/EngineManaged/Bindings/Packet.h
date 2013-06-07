/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Network/Packet.h>
#include "ResourceHandle.h"

namespace Flood
{
    enum struct PacketFlags : unsigned char;
    ref class _ENetPacket;
    ref class Packet;

    [System::Flags]
    public enum struct PacketFlags : unsigned char
    {
        Binary = 1,
        Encrypted = 2,
        Compressed = 4,
        Reliable = 8,
        Signed = 16
    };

    public ref class Packet
    {
    public:
        property ::Packet* NativePtr;

        Packet(::Packet* native);
        Packet(System::IntPtr native);
        Packet(unsigned short id);
        Flood::PacketFlags GetFlags();
        void SetFlags(Flood::PacketFlags v);
        Flood::PacketFlags GetProcessedFlags();
        void SetProcessedFlags(Flood::PacketFlags v);
        unsigned short GetId();
        int Size();
        void Clear();
        void Write(System::Collections::Generic::List<unsigned char>^ data);
        System::Collections::Generic::List<unsigned char>^ Read();
        static Flood::Packet^ Create(unsigned short id);
    };
}
