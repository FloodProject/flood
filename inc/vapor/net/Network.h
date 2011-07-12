/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Net/Message.h"
#include "Net/MessageHandler.h"
#include "Core/Concurrency.h"
#include "Core/Event.h"

struct _ENetHost;
typedef _ENetHost ENetHost;

NAMESPACE_BEGIN

//-----------------------------------//

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

	Event0<> onClientConnected;
	Event0<> onClientDisconnected; 
	Event0<> onMessageReceived;
	
protected:

	// Keeps the message handlers.
	typedef std::map< MessageType::Enum, MessageHandler* > MessageHandlersMap;
	MessageHandlersMap messageHandlers;

	// Queue of undispatched messages.
	ConcurrentQueue<MessagePtr> messages;

	// ENet context.
	ENetHost* host;
};

//-----------------------------------//

NAMESPACE_END