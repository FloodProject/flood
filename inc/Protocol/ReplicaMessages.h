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

struct Object;

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaCreateInstanceMessage)
struct API_PROTOCOL ReplicaCreateInstanceMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaCreateInstanceMessage)

	ReplicaCreateInstanceMessage()
		: localId(0), classId(0), instance(nullptr)
	{}
	
	uint16 localId;
	ClassId classId;
	Object* instance;
};

REFLECT_DECLARE_CLASS(ReplicaNewInstanceMessage)
struct API_PROTOCOL ReplicaNewInstanceMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaNewInstanceMessage)

	ReplicaNewInstanceMessage()
		: instanceId(0), instance(nullptr)
	{}
	
	uint64 instanceId;
	Object* instance;
};

#define REPLICA_INITIAL_ID 100

REFLECT_DECLARE_ENUM(ReplicaMessageIds)

struct API_PROTOCOL ReplicaMessageIds
{
	enum Enum : MessageId
	{
		ReplicaCreateInstance	= REPLICA_INITIAL_ID + 0,
		ReplicaNewInstance		= REPLICA_INITIAL_ID + 1,
	};
};

REFLECT_DECLARE_CLASS(ReplicaMessagePlugin)

//-----------------------------------//

class API_PROTOCOL ReplicaMessagePlugin : public MessagePlugin
{
	REFLECT_DECLARE_OBJECT(ReplicaMessagePlugin)

public:

	ReplicaMessagePlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Message callbacks.
	virtual void handleReplicaCreateInstance(const SessionPtr&, const ReplicaCreateInstanceMessage&) {}
	virtual void handleReplicaNewInstance(const SessionPtr&, const ReplicaNewInstanceMessage&) {}

	uint64 nextInstance;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END