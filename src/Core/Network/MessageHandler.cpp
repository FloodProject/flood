/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/MessageHandler.h"
#include "Core/Network/MessageHandlers.h"
#include "Core/Containers/Array.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_CLASS(MessageDefinition)
REFLECT_CLASS_END()

REFLECT_ABSTRACT_CHILD_CLASS(MessageHandler, Extension)
REFLECT_CLASS_END()

//-----------------------------------//

MessagesTable MessageHandler::NullMessageTable = Array<MessageMapping>(*AllocatorGetHeap());

//-----------------------------------//

ExtensionMetadata* MessageHandler::getMetadata()
{
	static ExtensionMetadata s_Extension =
	{
		"Message Handler",
		"Handles incoming network messages"
	};

	return &s_Extension;
}

//-----------------------------------//

NAMESPACE_CORE_END