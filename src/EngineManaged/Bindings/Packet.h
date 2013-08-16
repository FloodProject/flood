/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Packet.h>

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

    public ref class Packet : ICppInstance
    {
    public:

        property ::Packet* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Packet(::Packet* native);
        Packet(System::IntPtr native);
        Packet(unsigned short id);

        property Flood::PacketFlags Flags
        {
            Flood::PacketFlags get();
            void set(Flood::PacketFlags);
        }
        property Flood::PacketFlags ProcessedFlags
        {
            Flood::PacketFlags get();
            void set(Flood::PacketFlags);
        }
        property unsigned short Id
        {
            unsigned short get();
        }
        int Size();

        void Clear();

        void Write(System::Collections::Generic::List<unsigned char>^ data);

        System::Collections::Generic::List<unsigned char>^ Read();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
