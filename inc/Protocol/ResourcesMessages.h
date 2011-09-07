/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"
#include "Network/MessageHandler.h"
#include "Core/Event.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourcesAddressMessage)
struct API_PROTOCOL ResourcesAddressMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ResourcesAddressMessage)
	String address;
};

//-----------------------------------//

#define RESOURCES_INITIAL_ID 100

REFLECT_DECLARE_ENUM(ResourcesMessageIds)

struct API_PROTOCOL ResourcesMessageIds
{
	enum Enum : MessageId
	{
		ResourcesAddress	= RESOURCES_INITIAL_ID + 0,
	};
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourcesMessageHandler)
class API_PROTOCOL ResourcesMessageHandler : public MessageHandler
{
	REFLECT_DECLARE_STATIC_CLASS(ResourcesMessageHandler)

public:

	ResourcesMessageHandler() {}

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Message callbacks.
	virtual void handleResourcesAddress(const SessionPtr&, const ResourcesAddressMessage&);
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END