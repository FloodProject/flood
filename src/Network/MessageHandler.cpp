/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/MessageHandler.h"
#include "Network/MessageHandlers.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_CLASS(MessageDefinition)
REFLECT_CLASS_END()

REFLECT_ABSTRACT_CLASS(MessageHandler)
REFLECT_CLASS_END()

MessagesTable MessageHandler::NullMessageTable;

//-----------------------------------//

NAMESPACE_CORE_END