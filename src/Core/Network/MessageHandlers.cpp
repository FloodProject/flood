/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/MessageHandlers.h"
#include "Core/Containers/Hash.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

MessageHandlers::MessageHandlers()
	: handlers(*AllocatorGetHeap())
{}

//-----------------------------------//

MessageHandlers::~MessageHandlers()
{
	for(auto h : handlers)
		DeallocateObject(h.value);
}

//-----------------------------------//

void MessageHandlers::addHandlers(MessageHandler* handler, MessageDirection::Enum direction)
{
	const MessagesTable& messagesTable = handler->getMessagesTable();
	Enum* messagesEnum = handler->getMessagesEnum();

	for(size_t i = 0; i < messagesTable.size(); ++i )
	{
		MessageMapping mapping = messagesTable[i];

		bool isBoth = mapping.direction == MessageDirection::Both;
		if(!(isBoth || mapping.direction == direction))
			continue;

		mapping.handler = handler;
		addMapping(mapping);

		const char* name = EnumGetValueName(messagesEnum, mapping.id);
		LogDebug("Registered message handler: %s", name);
	}
}

//-----------------------------------//

void MessageHandlers::removeHandlers(MessageHandler* handler)
{
	const MessagesTable& messagesTable = handler->getMessagesTable();
	Enum* messagesEnum = handler->getMessagesEnum();

	for(size_t i = 0; i < messagesTable.size(); ++i)
	{
		const MessageMapping& mapping = messagesTable[i];
		removeMapping(mapping);
	}
}

//-----------------------------------//

MessageMapping* MessageHandlers::findHandler(MessageId id)
{
	return handlers.get((uint64)id, nullptr);
}

//-----------------------------------//

void MessageHandlers::addMapping(const MessageMapping& handler)
{
	auto h = handlers.get((uint64)handler.id, nullptr);
	if(h != nullptr)
		DeallocateObject(h);

	h = new (AllocatorAllocate(AllocatorGetHeap(), sizeof(MessageMapping), alignof(MessageMapping))) MessageMapping(handler);
	handlers.set((uint64)handler.id, h);
}

//-----------------------------------//

void MessageHandlers::removeMapping(const MessageMapping& handler)
{
	auto h = handlers.get((uint64)handler.id, nullptr);
	if(h != nullptr)
		handlers.remove((uint64)handler.id);
}

//-----------------------------------//

NAMESPACE_CORE_END