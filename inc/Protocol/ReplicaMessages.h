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

typedef uint64 InstanceId;

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

REFLECT_DECLARE_CLASS(ReplicatedObject)
struct ReplicatedObject : public Object
{
	REFLECT_DECLARE_OBJECT(ReplicatedObject)

	ReplicatedObject() : instanceId(0), instance(nullptr) {}

	InstanceId instanceId;
	Object* instance;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaNewInstanceMessage)
struct API_PROTOCOL ReplicaNewInstanceMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaNewInstanceMessage)
	InstanceId instanceId;
	Object* instance;
#if 0
	ReplicatedObject object;
#endif
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaAskUpdateMessage)
struct API_PROTOCOL ReplicaAskUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaAskUpdateMessage)
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaFullUpdateMessage)
struct API_PROTOCOL ReplicaFullUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaFullUpdateMessage)
	std::vector<ReplicatedObject> objects;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaFieldUpdateMessage)
struct API_PROTOCOL ReplicaFieldUpdateMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(ReplicaFieldUpdateMessage)

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
		ReplicaFieldUpdate	= REPLICA_INITIAL_ID + 4,
	};
};

REFLECT_DECLARE_CLASS(ReplicaMessagePlugin)

//-----------------------------------//

typedef std::map<const Object*, InstanceId> ReplicasMap;
typedef std::map<InstanceId, ReplicatedObject> ReplicasIdMap;

class API_PROTOCOL ReplicaManager
{
public:

	// Adds a new replicated instance.
	void addReplica(const ReplicatedObject&);

	// Removes a replicated instance.
	void removeReplica(const ReplicatedObject&);

	// Find an instance by id.
	ReplicatedObject* findInstanceById(InstanceId id);

	// Find instance id of an instance.
	bool findInstance(const Object* object, InstanceId& id);

	ReplicasMap instances;
	ReplicasIdMap instancesIds;
};

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

	// Processes a field update.
	void processFieldUpdate(const MessagePtr&);
	void registerObjects(const Object* object);

	// Message callbacks.
	virtual void handleReplicaCreate(const SessionPtr&, const ReplicaCreateMessage&) {}
	virtual void handleReplicaNewInstance(const SessionPtr&, const ReplicaNewInstanceMessage&) {}
	virtual void handleReplicaAskUpdate(const SessionPtr&, const ReplicaAskUpdateMessage&) {}
	virtual void handleReplicaFullUpdate(const SessionPtr&, const ReplicaFullUpdateMessage&) {}
	virtual void handleReplicaFieldUpdate(const SessionPtr&, const MessagePtr&) {}

	uint64 nextInstance;
	ReplicaManager replicas;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END