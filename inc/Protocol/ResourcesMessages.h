/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"
#include "Network/MessagePlugin.h"
#include "Core/Event.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourcesContextCreateMessage)
struct API_PROTOCOL ResourcesContextCreateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ResourcesContextCreateMessage)
	ResourcesContextCreateMessage() : localId(0) {}
	ResourcesLocalId localId;
	ClassId classId;
};

//-----------------------------------//

#define RESOURCES_INITIAL_ID 100

REFLECT_DECLARE_ENUM(ResourcesMessageIds)

struct API_PROTOCOL ResourcesMessageIds
{
	enum Enum : MessageId
	{
		ResourcesGetServer	= RESOURCES_INITIAL_ID + 0,
	};
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourcesMessagePlugin)
class API_PROTOCOL ResourcesMessagePlugin : public MessagePlugin
{
	REFLECT_DECLARE_OBJECT(ResourcesMessagePlugin)

public:

	ResourcesMessagePlugin() {}

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END