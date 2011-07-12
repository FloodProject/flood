/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#include "Net/Network.h"
#include "Net/Message.h"

#include "Core/Log.h"
#include "Core/Memory.h"

#include <enet/enet.h>

NAMESPACE_BEGIN

//-----------------------------------//

static Allocator* enet_alloc = AllocatorCreateHeap(AllocatorGetHeap(), "ENet");

static ENetCallbacks enet_callbacks;

static void* ENET_CALLBACK enet_custom_malloc(size_t size)
{
	return AllocatorAllocate(enet_alloc, size, 0);
}

static void ENET_CALLBACK enet_custom_free(void * memory)
{
	return AllocatorDeallocate(memory);
}

//-----------------------------------//

static bool InitializeENet()
{
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

	return true;
}

static void DeinitializeENet()
{
	enet_deinitialize();
	LogInfo("Deinitialized ENet");
}

static int g_InitializedENet = false;

//-----------------------------------//

NetworkHost::NetworkHost()
	: host(nullptr)
{
	if( !g_InitializedENet )
	{
		g_InitializedENet = true;
		InitializeENet();
		atexit(DeinitializeENet);
	}
}

//-----------------------------------//

NetworkHost::~NetworkHost()
{
	enet_host_destroy(host);
}

//-----------------------------------//

#define ENET_BANDWIDTH_AUTO 0

bool NetworkHost::createSocket( const String& address, int port )
{
	ENetAddress addr;
	addr.host = ENET_HOST_ANY;
	addr.port = port;

	int numClients = 32;
	int numChannels = 2;
	int numBandwidthIn = ENET_BANDWIDTH_AUTO;
	int numBandwidthOut = ENET_BANDWIDTH_AUTO;
	
	host = enet_host_create(&addr, numClients, numChannels, numBandwidthIn, numBandwidthOut);

	if( !host )
	{
		LogError("Error creating ENet host");
		return false;
	}

	return true;
}

//-----------------------------------//

void NetworkHost::waitMessages()
{
	ENetEvent event;

	// Wait up to 1000 milliseconds for an event.
	while(enet_host_service(host, &event, 1000) > 0)
	{
		MessagePtr message = new Message();
		message->init();

		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			LogInfo("A new client connected from %x:%u", 
					event.peer->address.host,
					event.peer->address.port);

			// Store any relevant client information here.
			event.peer->data = "Client information";

			break;

		case ENET_EVENT_TYPE_RECEIVE:
			LogInfo("A packet of length %u containing %s was received from %s on channel %u",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);

			//messages.push(message);

			// Clean up the packet now that we're done using it.
			enet_packet_destroy(event.packet);
			
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			LogInfo("%s disconected", event.peer -> data);

			// Reset the peer's client information.
			event.peer->data = nullptr;
		}
	}
}

//-----------------------------------//

void NetworkHost::dispatchMessages()
{
	MessagePtr msg;
	
	while(true)
	{
		messages.wait_and_pop(msg);

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

NAMESPACE_END