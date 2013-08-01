/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Host.h>

namespace Flood
{
    enum struct HostState;
    ref class _ENetEvent;
    ref class _ENetHost;
    ref class Host;
    ref class HostClient;
    ref class HostServer;
    ref class Packet;
    ref class PacketProcessors;
    ref class Peer;
    ref class Session;
    ref class SessionManager;
    value struct HostConnectionDetails;

    public enum struct HostState
    {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Disconnecting = 3
    };

    public ref class Host : ICppInstance
    {
    public:
        property ::Host* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Host(::Host* native);
        Host(System::IntPtr native);
    private:
        delegate void _PeerConnectDelegate(const ::PeerPtr& _0);
        _PeerConnectDelegate^ _PeerConnectDelegateInstance;
        void _PeerConnectRaise(const ::PeerPtr& _0);
        System::Action<Flood::Peer^>^ _PeerConnect;
    public:
        event System::Action<Flood::Peer^>^ PeerConnect
        {
            void add(System::Action<Flood::Peer^>^ evt);
            void remove(System::Action<Flood::Peer^>^ evt);
            void raise(Flood::Peer^ _0);
        }
    private:
        delegate void _PeerDisconnectDelegate(const ::PeerPtr& _1);
        _PeerDisconnectDelegate^ _PeerDisconnectDelegateInstance;
        void _PeerDisconnectRaise(const ::PeerPtr& _1);
        System::Action<Flood::Peer^>^ _PeerDisconnect;
    public:
        event System::Action<Flood::Peer^>^ PeerDisconnect
        {
            void add(System::Action<Flood::Peer^>^ evt);
            void remove(System::Action<Flood::Peer^>^ evt);
            void raise(Flood::Peer^ _1);
        }
    private:
        delegate void _PeerPacketDelegate(const ::PeerPtr& _2, const ::PacketPtr& _3, int _4);
        _PeerPacketDelegate^ _PeerPacketDelegateInstance;
        void _PeerPacketRaise(const ::PeerPtr& _2, const ::PacketPtr& _3, int _4);
        System::Action<Flood::Peer^, Flood::Packet^, int>^ _PeerPacket;
    public:
        event System::Action<Flood::Peer^, Flood::Packet^, int>^ PeerPacket
        {
            void add(System::Action<Flood::Peer^, Flood::Packet^, int>^ evt);
            void remove(System::Action<Flood::Peer^, Flood::Packet^, int>^ evt);
            void raise(Flood::Peer^ _2, Flood::Packet^ _3, int _4);
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
        System::String^ Address;
        unsigned short Port;
        unsigned char ChannelCount;
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
