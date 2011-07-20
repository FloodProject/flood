/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/MessageHandlers.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

MessageHandlers::MessageHandlers()
{
}

//-----------------------------------//

MessageHandlers::~MessageHandlers()
{
}

//-----------------------------------//

void MessageHandlers::addHandler(const MessageHandler& handler)
{
	MessageId id = handler.id;
	handlers[id] = handler;
}

//-----------------------------------//

void MessageHandlers::removeHandler(const MessageHandler& handler)
{
	MessageHandlersMap::iterator it = handlers.find(handler.id);

	if( it == handlers.end() )
		return;

	handlers.erase(it);
}

//-----------------------------------//

NAMESPACE_CORE_END