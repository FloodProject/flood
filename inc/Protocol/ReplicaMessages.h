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

REFLECT_DECLARE_CLASS(ReplicaCreateMessage)
struct API_PROTOCOL ReplicaCreateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaCreateMessage)

	ReplicaCreateMessage()
		: localId(0), classId(0), instance(nullptr)
	{}
	
	uint16 localId;
	ClassId classId;
	Object* instance;
};

//-----------------------------------//

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

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaAskUpdateMessage)
struct API_PROTOCOL ReplicaAskUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaAskUpdateMessage)
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicatedObject)
struct ReplicatedObject : public Object
{
	REFLECT_DECLARE_OBJECT(ReplicatedObject)
	uint64 instanceId;
	Object* instance;
};

REFLECT_DECLARE_CLASS(ReplicaFullUpdateMessage)
struct API_PROTOCOL ReplicaFullUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaFullUpdateMessage)
	std::vector<ReplicatedObject> objects;
};

//-----------------------------------//

#define REPLICA_INITIAL_ID 100

REFLECT_DECLARE_ENUM(ReplicaMessageIds)

struct API_PROTOCOL ReplicaMessageIds
{
	enum Enum : MessageId
	{
		ReplicaCreate		= REPLICA_INITIAL_ID + 0,
		ReplicaNewInstance	= REPLICA_INITIAL_ID + 1,
		ReplicaAskUpdate	= REPLICA_INITIAL_ID + 2,
		ReplicaFullUpdate	= REPLICA_INITIAL_ID + 3,
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
	virtual void handleReplicaCreate(const SessionPtr&, const ReplicaCreateMessage&) {}
	virtual void handleReplicaNewInstance(const SessionPtr&, const ReplicaNewInstanceMessage&) {}
	virtual void handleReplicaAskUpdate(const SessionPtr&, const ReplicaAskUpdateMessage&) {}
	virtual void handleReplicaFullUpdate(const SessionPtr&, const ReplicaFullUpdateMessage&) {}

	uint64 nextInstance;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END