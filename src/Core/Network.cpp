/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#include "Core/Log.h"
#include "Core/Memory.h"

#include "Net/Network.h"
#include "Net/Message.h"

#include <enet/enet.h>

NAMESPACE_BEGIN

//-----------------------------------//

static Allocator* enet_alloc = nullptr;

static void* ENET_CALLBACK enet_custom_malloc(size_t size)
{
	return AllocatorAllocate(enet_alloc, size, 0);
}

//-----------------------------------//

static void ENET_CALLBACK enet_custom_free(void * memory)
{
	return AllocatorDeallocate(memory);
}

//-----------------------------------//

bool NetworkInitialize()
{
	static int initializedENet = false;
	static ENetCallbacks enet_callbacks;
	
	if( initializedENet )
		return true;

	assert(enet_alloc == nullptr);
	enet_alloc = AllocatorCreateHeap(AllocatorGetHeap(), "ENet");

	enet_callbacks.malloc = enet_custom_malloc;
	enet_callbacks.free = enet_custom_free;

    int ret = enet_initialize_with_callbacks(ENET_VERSION, &enet_callbacks);

	if(ret != 0)
	{
		LogError("Error initializing ENet");
		return false;
	}

	LogInfo("Initialized ENet %d.%d.%d networking layer",
		ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH);

	initializedENet = true;

	return true;
}

//-----------------------------------//

void NetworkDeinitialize()
{
	enet_deinitialize();
	
	AllocatorDestroy(enet_alloc);
	enet_alloc = nullptr;

	LogInfo("Deinitialized ENet");
}

//-----------------------------------//

#define ENET_BANDWIDTH_AUTO 0

static bool CreateEnetSocket( const String& address, int port, ENetHost*& host, bool server )
{
	ENetAddress addr;
	addr.host = ENET_HOST_ANY;
	addr.port = port;

	int numClients = 32;
	int numChannels = 2;
	int numBandwidthIn = ENET_BANDWIDTH_AUTO;
	int numBandwidthOut = ENET_BANDWIDTH_AUTO;
	
	ENetAddress* enetAddress = server ? &addr : nullptr;

	host = enet_host_create(enetAddress, numClients, numChannels, numBandwidthIn, numBandwidthOut);

	if( !host )
	{
		LogError("Error creating ENet host");
		return false;
	}

	return true;
}

//-----------------------------------//

NetworkHost::NetworkHost()
	: host(nullptr)
{
}

//-----------------------------------//

NetworkHost::~NetworkHost()
{
	enet_host_destroy(host);
}

//-----------------------------------//

bool NetworkHost::createSocket( const String& address, int port )
{
	return CreateEnetSocket(address, port, host, true);
}

//-----------------------------------//

void NetworkHost::waitMessages()
{
	ENetEvent event;

	// Wait up to 1000 milliseconds for an event.
	while(enet_host_service(host, &event, 1000) > 0)
	{
		MessagePtr message = Allocate(Message, AllocatorGetHeap());
		message->init();

		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			ENetPeer* newPeer = event.peer;

			char ip[64];
			enet_address_get_host_ip(&newPeer->address, ip, ARRAY_SIZE(ip));

			LogInfo("A new client connected from %s:%hu",  ip, newPeer->address.port);

			NetworkPeerPtr networkPeer = Allocate(NetworkPeer, AllocatorGetHeap());
			networkPeer->peer = event.peer;

			// Store the network peer in ENet as user data.
			event.peer->data = networkPeer.get();

			peers.push_back(networkPeer);
			
			onClientConnected(networkPeer);
			
			break;
		}
		case ENET_EVENT_TYPE_RECEIVE:
		{
			LogInfo("A packet of length %u containing %s was received from %s on channel %u",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);

			//messages.push(message);

			// Clean up the packet now that we're done using it.
			enet_packet_destroy(event.packet);
			
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			ENetPeer* peer = event.peer;

			char ip[64];
			enet_address_get_host_ip(&peer->address, ip, ARRAY_SIZE(ip));

			LogInfo("%s disconnected", ip);

			NetworkPeer* networkPeer = (NetworkPeer*) event.peer->data;
			//networkPeer->disconnect();

			onClientDisconnected(networkPeer);

			// Reset the peer's client information.
			event.peer->data = nullptr;
		} }
	}
}

//-----------------------------------//

void NetworkHost::dispatchMessages()
{
	MessagePtr msg;
	
	while( messages.try_pop(msg) )
	{
		MessageType::Enum type = msg->getMessageType();
		MessageHandlersMap::iterator it = messageHandlers.find(type);
		
		if( it == messageHandlers.end() )
		{
			LogWarn("No message handler for type: %d", type);
			continue;
		}

		MessageHandler* handler = (*it).second;
		assert(handler != nullptr);
		
		handler->handleMessage(msg);
	}
}

//-----------------------------------//

void NetworkHost::sendMessage( const MessagePtr& message )
{
#if 0
	int status = zmq_send(socket, message->getBuffer(), 0);

	if( status != 0 )
		error("Error sending message");
#endif
}

//-----------------------------------//

String NetworkPeer::getHostname() const
{
	char name[256];
	
	if(enet_address_get_host(&peer->address, name, ARRAY_SIZE(name)) < 0)
	{
		LogError("Could not get hostname of network peer");
		return "";
	}

	return String(name);
}

NetworkPeer::NetworkPeer()
	: peer(nullptr)
{

}

//-----------------------------------//

NetworkClient::NetworkClient()
	: client(nullptr)
{

}

//-----------------------------------//

bool NetworkClient::connect( const String& address, int port )
{
	if( !CreateEnetSocket(address, port, client, false) )
		return false;

	ENetAddress addr;
	addr.port = port;

	enet_address_set_host( &addr, address.c_str() );

	ENetPeer* peer = enet_host_connect(client, &addr, 2, 0);

    // Wait up to 5 seconds for the connection attempt to succeed.

	ENetEvent event;
    
	if(!(enet_host_service (client, &event, 5000) > 0))
		return false;
	
	if(event.type == ENET_EVENT_TYPE_CONNECT)
    {
        LogInfo ("Connection to some.server.net:1234 succeeded.");
    }
    else
    {
        enet_peer_reset (peer);
        LogError("Connection to some.server.net:1234 failed.");
		return false;
    }

	return true;
}

//-----------------------------------//

void NetworkClient::checkEvents(uint32 timeout)
{
	ENetEvent event;
	
	while(enet_host_service(client, &event, timeout) > 0)
	{
		switch(event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:

			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			break;
		};

		LogInfo("Received event from: %s", event.peer->address);
	}
}

//-----------------------------------//

NAMESPACE_END