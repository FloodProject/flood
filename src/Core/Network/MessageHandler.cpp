/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_NETWORK_ENET

#include "Core/Network/MessageHandler.h"
#include "Core/Network/MessageHandlers.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_CLASS(MessageDefinition)
REFLECT_CLASS_END()

REFLECT_ABSTRACT_CHILD_CLASS(MessageHandler, Extension)
REFLECT_CLASS_END()

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

MessagesTable MessageHandler::NullMessageTable;

//-----------------------------------//

NAMESPACE_CORE_END

#endif