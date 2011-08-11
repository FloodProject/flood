/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Network/Message.h"
#include "Network/MessagePlugin.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef void (*MessageRefHandlerFn)(MessagePlugin*, const SessionPtr&, Object*);
typedef void (MessagePlugin::*MessageRawHandlerMemFn)(const SessionPtr&, const MessagePtr&);

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
	MessagePlugin* plugin;
};

typedef std::map<MessageId, MessageMapping> MessageHandlersMap;

class MessageHandlers
{
public:

	MessageHandlers();
	~MessageHandlers();

	// Adds a message handler.
	void addHandler(const MessageMapping&);

	// Removes a message handler.
	void removeHandler(const MessageMapping&);

	// Finds a message handler.
	MessageMapping* findHandler(MessageId id);

	// Gets the map with the message handlers.
	GETTER(MessageHandlers, const MessageHandlersMap&, handlers)

protected:
	
	MessageHandlersMap handlers;
	PluginManager* plugins;
};

//-----------------------------------//

NAMESPACE_CORE_END