/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Network/Host.h>
#include "ResourceHandle.h"

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

    public ref class Host
    {
    public:
        property ::Host* NativePtr;

        Host(::Host* native);
        Host(System::IntPtr native);
    private:
        delegate void _PeerConnectDelegate(const ::PeerPtr& _1);
        _PeerConnectDelegate^ _PeerConnectDelegateInstance;
        void _PeerConnectRaise(const ::PeerPtr& _1);
        System::Action<Flood::Peer^>^ _PeerConnect;
    public:
        event System::Action<Flood::Peer^>^ PeerConnect
        {
            void add(System::Action<Flood::Peer^>^ evt);
            void remove(System::Action<Flood::Peer^>^ evt);
            void raise(Flood::Peer^ _1);
        }
    private:
        delegate void _PeerDisconnectDelegate(const ::PeerPtr& _3);
        _PeerDisconnectDelegate^ _PeerDisconnectDelegateInstance;
        void _PeerDisconnectRaise(const ::PeerPtr& _3);
        System::Action<Flood::Peer^>^ _PeerDisconnect;
    public:
        event System::Action<Flood::Peer^>^ PeerDisconnect
        {
            void add(System::Action<Flood::Peer^>^ evt);
            void remove(System::Action<Flood::Peer^>^ evt);
            void raise(Flood::Peer^ _3);
        }
    private:
        delegate void _PeerPacketDelegate(const ::PeerPtr& _4, const ::PacketPtr& _5, int _7);
        _PeerPacketDelegate^ _PeerPacketDelegateInstance;
        void _PeerPacketRaise(const ::PeerPtr& _4, const ::PacketPtr& _5, int _7);
        System::Action<Flood::Peer^, Flood::Packet^, int>^ _PeerPacket;
    public:
        event System::Action<Flood::Peer^, Flood::Packet^, int>^ PeerPacket
        {
            void add(System::Action<Flood::Peer^, Flood::Packet^, int>^ evt);
            void remove(System::Action<Flood::Peer^, Flood::Packet^, int>^ evt);
            void raise(Flood::Peer^ _4, Flood::Packet^ _5, int _7);
        }
        bool DestroySocket();
        void BroadcastPacket(Flood::Packet^ _0, unsigned char channel);
        void ProcessEvents(unsigned int timeout);
        bool HasContext();
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
        bool Connect(Flood::HostConnectionDetails _0);
        Flood::Peer^ GetPeer();
        Flood::Session^ GetSession();
    };

    public ref class HostServer : Flood::Host
    {
    public:
        HostServer(::HostServer* native);
        HostServer(System::IntPtr native);
        HostServer();
        bool CreateSocket(Flood::HostConnectionDetails _0);
        System::Collections::Generic::List<Flood::Peer^>^ GetPeers();
        Flood::SessionManager^ GetSessionManager();
    };
}
