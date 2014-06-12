/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Host.h>
#include "StringConverter.h"

namespace Flood
{
    ref class Host;
    ref class HostClient;
    ref class HostServer;
    ref class Packet;
    ref class Peer;
    ref class Session;
    ref class SessionManager;
    value struct HostConnectionDetails;
}

namespace Flood
{
    public ref class Host : ICppInstance
    {
    public:

        property ::Host* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Host(::Host* native);
        Host(System::IntPtr native);
    private:
        delegate void _SessionPacketDelegate(::Session* _0, const ::PacketPtr& _1, int _2);
        _SessionPacketDelegate^ _SessionPacketDelegateInstance;
        void _SessionPacketRaise(::Session* _0, const ::PacketPtr& _1, int _2);
        System::Action<Flood::Session^, Flood::Packet^, int>^ _SessionPacket;
    public:
        event System::Action<Flood::Session^, Flood::Packet^, int>^ SessionPacket
        {
            void add(System::Action<Flood::Session^, Flood::Packet^, int>^ evt);
            void remove(System::Action<Flood::Session^, Flood::Packet^, int>^ evt);
            void raise(Flood::Session^ _0, Flood::Packet^ _1, int _2);
        }
        bool DestroySocket();

        void BroadcastPacket(Flood::Packet^ _0, unsigned char channel);

        void ProcessEvents(unsigned int timeout);

        bool HasContext();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public value struct HostConnectionDetails
    {
    public:

        HostConnectionDetails(::HostConnectionDetails* native);
        HostConnectionDetails(System::IntPtr native);
        HostConnectionDetails(System::String^ address, unsigned short port, unsigned char channelCount);

        property System::String^ Address
        {
            System::String^ get();
            void set(System::String^);
        }

        property unsigned short Port
        {
            unsigned short get();
            void set(unsigned short);
        }

        property unsigned char ChannelCount
        {
            unsigned char get();
            void set(unsigned char);
        }

        private:
        System::String^ __Address;
        unsigned short __Port;
        unsigned char __ChannelCount;
    };

    public ref class HostClient : Flood::Host
    {
    public:

        HostClient(::HostClient* native);
        HostClient(System::IntPtr native);
        HostClient();

        property Flood::Peer^ Peer
        {
            Flood::Peer^ get();
        }

        property Flood::Session^ Session
        {
            Flood::Session^ get();
        }

        bool Connect(Flood::HostConnectionDetails _0);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class HostServer : Flood::Host
    {
    public:

        HostServer(::HostServer* native);
        HostServer(System::IntPtr native);
        HostServer();

        property System::Collections::Generic::List<Flood::Peer^>^ Peers
        {
            System::Collections::Generic::List<Flood::Peer^>^ get();
        }

        property Flood::SessionManager^ SessionManager
        {
            Flood::SessionManager^ get();
        }

        bool CreateSocket(Flood::HostConnectionDetails _0);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
