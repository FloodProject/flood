/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/MessageDispatcher.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/PluginManager.h"
#include "Core/Serialization.h"
#include "Core/Stream.h"

#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Message.h"
#include "Network/MessageHandlers.h"
#include "Network/Session.h"
#include "Network/SessionManager.h"

#include <fastlz.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

MessageDispatcher::MessageDispatcher()
	: isServer(false)
	, sessions(nullptr)
	, handlers(nullptr)
	, serializer(nullptr)
{
	sessions = AllocateThis(SessionManager);
	handlers = AllocateThis(MessageHandlers);
	serializer = SerializerCreateBinary(AllocatorGetThis());
}

//-----------------------------------//

void MessageDispatcher::initClient(HostClient* host)
{
	isServer = false;
	host->onClientConnected.Connect(this, &MessageDispatcher::handleConnect);
	host->onClientDisconnected.Connect(this, &MessageDispatcher::handleDisconnect);
	host->onServerMessage.Connect(this, &MessageDispatcher::handleMessage);
}

//-----------------------------------//

void MessageDispatcher::initServer(HostServer* host)
{
	isServer = true;
	host->onClientConnected.Connect(this, &MessageDispatcher::handleConnect);
	host->onClientDisconnected.Connect(this, &MessageDispatcher::handleDisconnect);
	host->onClientMessage.Connect(this, &MessageDispatcher::handleMessage);
}

//-----------------------------------//

MessageDispatcher::~MessageDispatcher()
{
	Deallocate(sessions);
	Deallocate(handlers);
	Deallocate(serializer);
}

//-----------------------------------//

void MessageDispatcher::handleConnect(const PeerPtr& peer)
{
	SessionPtr session = AllocateThis(Session);
	session->setPeer(peer);

	sessions->addSession(session);
}

//-----------------------------------//

void MessageDispatcher::handleDisconnect(const PeerPtr& peer)
{
	const SessionPtr& session = sessions->getSession(peer);
	assert( session != nullptr );

	sessions->removeSession(session);
}

//-----------------------------------//

void MessageDispatcher::handleMessage(const PeerPtr& peer, const MessagePtr& message)
{
	const SessionPtr& session = sessions->getSession(peer);

	MessageRequest req;
	req.message = message;
	req.session = session;
	
	messages.push_back(req);
}

//-----------------------------------//

#define CALL_MEMBER_FN(object, ptrToMember)  ((object)->*(ptrToMember)) 

bool MessageDispatcher::processMessage()
{
	MessageRequest req;
	
	if( !messages.try_pop_front(req) )
		return false;

	const MessagePtr& message = req.message;
	const SessionPtr& session = req.session;

	// Find message handler and forward message.
	MessageMapping* mapping = handlers->findHandler(message->id);
	
	if( !mapping )
	{
		LogError("No message handler was found");
		return false;
	}

	StreamSetPosition(message->ms, 0, StreamSeekMode::Absolute);

	// Deserialize message if it's needed.
	if( GetBitFlag(message->flags, MessageFlags::Binary) )
	{
		if( !mapping->ref )
		{
			LogAssert("Handler was not found for reflected message");
			return false;
		}

		serializer->stream = message->ms;
		
		Object* object = SerializerLoad(serializer);
		if( !object ) return false;

		mapping->ref(mapping->handler, session, object);
		
		Deallocate(object);
	}
	else
	{
		if( !mapping->raw )
		{
			LogAssert("Handler was not found for raw message");
			return false;
		}

		CALL_MEMBER_FN(mapping->handler, mapping->raw)(session, message);
	}

	return true;
}

//-----------------------------------//

NAMESPACE_CORE_END