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

struct MessageHandler
{
	MessageId id;
	MessageHandlerMemFn fn;
	MessagePlugin* plugin;
};

typedef std::map<MessageId, MessageHandler> MessageHandlersMap;

class MessageHandlers
{
public:

	MessageHandlers();
	~MessageHandlers();

	// Adds a message handler.
	void addHandler(const MessageHandler&);

	// Removes a message handler.
	void removeHandler(const MessageHandler&);

	// Gets the map with the message handlers.
	GETTER(MessageHandlers, const MessageHandlersMap&, handlers)

protected:
	
	MessageHandlersMap handlers;
};

//-----------------------------------//

NAMESPACE_CORE_END