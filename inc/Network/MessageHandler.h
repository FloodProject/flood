/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Extension.h"
#include "Network/Message.h"


FWD_DECL_INTRUSIVE(Session)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct MessageMapping;
typedef std::vector<MessageMapping> MessagesTable;

REFLECT_DECLARE_CLASS(MessageHandler)

class API_CORE MessageHandler :  public Extension
{
public:

	// Gets metadata about this extension.
	ExtensionMetadata* getMetadata() OVERRIDE;

	// Network callbacks.
	virtual void onSessionAdded(const SessionPtr&) {}
	virtual void onSessionRemoved(const SessionPtr&) {}

	// Gets the messages that this plugin handles.
	virtual const MessagesTable& getMessagesTable() { return NullMessageTable; }

	// Gets the enum with the messages that this plugin handles.
	virtual Enum* getMessagesEnum() { return nullptr; }

	static MessagesTable NullMessageTable;
};

REFLECT_DECLARE_CLASS(MessageDefinition)

struct API_CORE MessageDefinition : public Object
{
	REFLECT_DECLARE_OBJECT(MessageDefinition)
};

//-----------------------------------//

NAMESPACE_CORE_END