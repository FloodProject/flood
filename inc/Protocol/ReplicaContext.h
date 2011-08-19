/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"
#include "Core/ClassWatcher.h"

struct Object;
FWD_DECL_INTRUSIVE(Sesison)

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

typedef uint16 ReplicaInstanceId;
typedef uint16 ReplicaLocalId;

REFLECT_DECLARE_CLASS(ReplicatedObject)
struct ReplicatedObject : public Object
{
	REFLECT_DECLARE_OBJECT(ReplicatedObject)
	ReplicatedObject() : instanceId(0), instance(nullptr) {}
	ReplicaInstanceId instanceId;
	Object* instance;
};

//-----------------------------------//

typedef uint8 ReplicaContextId;

typedef std::map<const Object*, ReplicaInstanceId> ReplicasMap;
typedef std::map<ReplicaInstanceId, ReplicatedObject> ReplicasIdMap;

class API_PROTOCOL ReplicaContext
{
public:

	ReplicaContext();

	// Adds a new replicated instance.
	void addReplica(const ReplicatedObject&);

	// Removes a replicated instance.
	void removeReplica(const ReplicatedObject&);

	// Find an instance by id.
	ReplicatedObject* findInstanceById(ReplicaInstanceId id);

	// Find instance id of an instance.
	bool findInstance(const Object* object, ReplicaInstanceId& id);

	// Registers object instances.
	void registerObjects(const Object* object);

	// Sends all the objects state.
	void sendObjects(const SessionPtr& session);

	// Creates an object update message.
	MessagePtr createObjectUpdateMessage(const FieldWatchVector& watches);

	// Processes a field update.
	void processFieldUpdate(const MessagePtr&);

	// Gets called when a replica is added.
	Delegate1<const ReplicatedObject&> onReplicaAdded;

	ReplicaContextId id;
	ReplicasMap instances;
	ReplicasIdMap instancesIds;
	ReplicaInstanceId nextInstance;
};

typedef std::map<ReplicaContextId, ReplicaContext*> ReplicaContextMap;

//-----------------------------------//

NAMESPACE_PROTOCOL_END