/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/ConcurrentQueue.h"

FWD_DECL_INTRUSIVE(Message)
FWD_DECL_INTRUSIVE(Peer)
FWD_DECL_INTRUSIVE(Session)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Class;
struct Serializer;

class Host;
class HostClient;
class HostServer;

class MessageHandler;
class MessageHandlers;
class SessionManager;

struct MessageRequest
{
	MessagePtr message;
	SessionPtr session;
};

// Dispatches messages to the their handlers.
class Dispatcher
{
public:

	Dispatcher();
	~Dispatcher();

	// Initializes for client mode.
	void initClient(HostClient*);

	// Initializes for server mode.
	void initServer(HostServer*);

	// Processes one message or returns false if there is none.
	bool processMessage();

	// Gets the host.
	GETTER(Host, Host*, host)
	
	// Gets the session manager.
	GETTER(SessionManager, SessionManager*, sessions)

	// Gets the message handlers.
	GETTER(MessageHandlers, MessageHandlers*, handlers)

protected:

	// Handles host callbacks.
	void handleConnect(const PeerPtr&);
	void handleDisconnect(const PeerPtr&);
	void handleMessage(const PeerPtr&, const MessagePtr&);

	Host* host;
	bool isServer;

	Serializer* serializer;
	SessionManager* sessions;
	MessageHandlers* handlers;

	ConcurrentQueue<MessageRequest> messages;
};

//-----------------------------------//

NAMESPACE_CORE_END