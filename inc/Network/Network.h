/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"
#include "Core/Event.h"

#include "Network/Message.h"
#include "Network/MessageHandler.h"

struct _ENetHost;
typedef _ENetHost ENetHost;

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

NAMESPACE_BEGIN

//-----------------------------------//

// Clients connect to hosts.
class NetworkClient : public ReferenceCounted
{
public:

	NetworkClient();
	
	// Connects the client to a remote host.
	bool connect( const String& address, int port );

	// Checks the client for events.
	void checkEvents(uint32 timeout);

	ENetHost* client;
	ENetPeer* peer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( NetworkClient )
typedef std::vector<NetworkClientPtr> NetworkClients;

// Peers are clients seen from the server.
class NetworkPeer : public ReferenceCounted
{
public:

	NetworkPeer();

	// Gets the peer address as IP.
	String getHostname() const;

	ENetPeer* peer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( NetworkPeer )
typedef std::vector<NetworkPeerPtr> NetworkPeers;

// Hosts are the server itself that accepts clients and services peers.
class NetworkHost
{
public:

	NetworkHost();
	~NetworkHost();

	// Creates a new socket.
	bool createSocket( const String& address, int port );

	// Sends a message to the client.
	void sendMessage( const MessagePtr& message );

	// Waits for messages.
	void waitMessages();

	// Dispatchs messages to their handlers.
	void dispatchMessages();

	Event1<const NetworkPeerPtr&> onClientConnected;
	Event1<const NetworkPeerPtr&> onClientDisconnected; 
	Event0<> onMessageReceived;
	
protected:

	// Keeps the message handlers.
	typedef std::map< MessageType::Enum, MessageHandler* > MessageHandlersMap;
	MessageHandlersMap messageHandlers;

	// Queue of undispatched messages.
	ConcurrentQueue<MessagePtr> messages;

	// Network peers.
	NetworkPeers peers;

	// ENet context.
	ENetHost* host;
};

API_CORE bool NetworkInitialize();
API_CORE void NetworkDeinitialize();

//-----------------------------------//

NAMESPACE_END