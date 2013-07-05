/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Network/Message.h"
#include "Core/Network/MessageHandler.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef void (*MessageRefHandlerFn)(MessageHandler*, const SessionPtr&, Object*);
typedef void (MessageHandler::*MessageRawHandlerMemFn)(const SessionPtr&, const MessagePtr&);

struct MessageDirection
{
	enum Enum
	{
		ClientToServer,
		ServerToClient,
		Both,
		ServerToServer,
	};
};

struct MessageMapping
{
	MessageId id;
	MessageDirection::Enum direction;
	MessageRefHandlerFn ref;
	MessageRawHandlerMemFn raw;
	MessageHandler* handler;
};

typedef Hash<MessageMapping*> MessageHandlersMap;

class MessageHandlers
{
public:

	MessageHandlers();
	~MessageHandlers();

	// Adds a message handler.
	void addHandlers(MessageHandler* handler, MessageDirection::Enum);

	// Removes a message handler.
	void removeHandlers(MessageHandler* handler);

	// Adds a message mapping.
	void addMapping(const MessageMapping&);

	// Removes a message mapping.
	void removeMapping(const MessageMapping&);

	// Finds a message handler.
	MessageMapping* findHandler(MessageId id);

	// Gets the map with the message handlers.
	GETTER(MessageHandlers, const MessageHandlersMap&, handlers)

protected:
	
	MessageHandlersMap handlers;
};

//-----------------------------------//

NAMESPACE_CORE_END