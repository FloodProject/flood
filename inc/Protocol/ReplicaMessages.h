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
#include "Protocol/ReplicaContext.h"
#include "Core/Event.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaContextCreateMessage)
struct API_PROTOCOL ReplicaContextCreateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaContextCreateMessage)
	ReplicaContextCreateMessage() : localId(0) {}
	ReplicaLocalId localId;
	ClassId classId;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaContextCreatedMessage)
struct API_PROTOCOL ReplicaContextCreatedMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaContextCreatedMessage)
	ReplicaContextCreatedMessage() : contextId(0), localId(0) {}
	ReplicaContextId contextId;
	ReplicaLocalId localId;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaContextRequestMessage)
struct API_PROTOCOL ReplicaContextRequestMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaContextRequestMessage)
	ReplicaContextRequestMessage() : contextId(0) {}
	ReplicaContextId contextId;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaContextUpdateMessage)
struct API_PROTOCOL ReplicaContextUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaContextUpdateMessage)
	ReplicaContextUpdateMessage() : contextId(0) {}
	ReplicaContextId contextId;
	std::vector<ReplicatedObject> objects;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaObjectUpdateMessage)
struct API_PROTOCOL ReplicaObjectUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaObjectUpdateMessage)
	// This message is not explicitly typed.
};

//-----------------------------------//

struct Object;

REFLECT_DECLARE_CLASS(ReplicaObjectCreateMessage)
struct API_PROTOCOL ReplicaObjectCreateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaObjectCreateMessage)
	ReplicaObjectCreateMessage() : contextId(0), localId(0), classId(0), instance(nullptr) {}
	ReplicaContextId contextId;
	ReplicaLocalId localId;
	ReplicaInstanceId parentId;
	ClassId classId;
	Object* instance;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaObjectCreatedMessage)
struct API_PROTOCOL ReplicaObjectCreatedMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaObjectCreatedMessage)
	ReplicaContextId contextId;
	ReplicaLocalId localId;
	ReplicaInstanceId instanceId;
	ReplicaInstanceId parentId;
	Object* instance;
#if 0
	ReplicatedObject object;
#endif
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaObjectDeleteMessage)
struct API_PROTOCOL ReplicaObjectDeleteMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaObjectDeleteMessage)
	ReplicaObjectDeleteMessage() : contextId(0), instanceId(0) {}
	ReplicaContextId contextId;
	ReplicaInstanceId instanceId;
};

//-----------------------------------//

#define REPLICA_INITIAL_ID 100

REFLECT_DECLARE_ENUM(ReplicaMessageIds)

struct API_PROTOCOL ReplicaMessageIds
{
	enum Enum : MessageId
	{
		ReplicaContextCreate	= REPLICA_INITIAL_ID + 0,
		ReplicaContextCreated	= REPLICA_INITIAL_ID + 1,
		ReplicaContextRequest	= REPLICA_INITIAL_ID + 2,
		ReplicaContextUpdate	= REPLICA_INITIAL_ID + 3,

		ReplicaObjectCreate		= REPLICA_INITIAL_ID + 4,
		ReplicaObjectCreated	= REPLICA_INITIAL_ID + 5,
		ReplicaObjectUpdate		= REPLICA_INITIAL_ID + 6,
		ReplicaObjectDelete		= REPLICA_INITIAL_ID + 7,
	};
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaMessagePlugin)
class API_PROTOCOL ReplicaMessagePlugin : public MessagePlugin
{
	REFLECT_DECLARE_OBJECT(ReplicaMessagePlugin)

public:

	ReplicaMessagePlugin() : nextContextId(0) {}

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Finds an existing replica context.
	ReplicaContext* findContext(ReplicaContextId id);

	// Message callbacks.
	virtual void handleReplicaContextCreate(const SessionPtr&, const ReplicaContextCreateMessage&) {}
	virtual void handleReplicaContextCreated(const SessionPtr&, const ReplicaContextCreatedMessage&) {}
	virtual void handleReplicaContextRequest(const SessionPtr&, const ReplicaContextRequestMessage&) {}
	virtual void handleReplicaContextUpdate(const SessionPtr&, const ReplicaContextUpdateMessage&) {}
	
	virtual void handleReplicaObjectCreate(const SessionPtr&, const ReplicaObjectCreateMessage&) {}
	virtual void handleReplicaObjectDelete(const SessionPtr&, const ReplicaObjectDeleteMessage&) {}
	virtual void handleReplicaObjectUpdate(const SessionPtr&, const MessagePtr&);
	virtual void handleReplicaObjectCreated(const SessionPtr&, const ReplicaObjectCreatedMessage&) {}

	ReplicaContextMap replicaContexts;
	ReplicaContextId nextContextId;

	Event3<ReplicaContext*, ClassId, ReplicaLocalId> onReplicaContextCreate;
	Event3<ReplicaContext*, ReplicaInstanceId, Object*> onReplicaObjectCreate;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END