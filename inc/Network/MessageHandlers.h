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

typedef void (MessagePlugin::*MessageHandlerMemFn)(const SessionPtr&, const MessagePtr&);

struct MessageDirection
{
	enum Enum
	{
		ClientToServer,
		ServerToClient,
		ServerToServer,
	};
};

struct MessageMapping
{
	MessageId id;
	MessageDirection::Enum direction;
	MessageHandlerMemFn handler;
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