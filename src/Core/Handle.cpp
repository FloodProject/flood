/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Handle.h"
#include "Core/Log.h"
#include "Core/Containers/Hash.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

HandleManager::HandleManager()
	: handles(*AllocatorGetHeap())
{}

//-----------------------------------//

HandleManager* HandleCreateManager( Allocator* alloc )
{
	HandleManager* man = Allocate(alloc, HandleManager);
	man->nextHandle = 0;
	return man;
}

//-----------------------------------//

void HandleDestroyManager( HandleManager* man )
{
	if( !man ) return;

	const HandleMap& handles = man->handles;

	if( hash::size(handles) > 0 )
	{
		//LogAssert("Handle manager should not have any handles");
		goto out;
	}

out:

	Deallocate(man);
}

//-----------------------------------//

HandleId HandleCreate(HandleManager* man, ReferenceCounted* ref)
{
	if( !man ) return HandleInvalid;
	
	HandleMap& handles = man->handles;
	
	HandleId id = AtomicIncrement(&man->nextHandle);
	hash::set(handles, (uint64)id, ref);
	
	return id;
}

//-----------------------------------//

void HandleDestroy(HandleManager* man, HandleId id)
{
	if( !man ) return;
	
	hash::remove(man->handles, (uint64)id);
}

//-----------------------------------//

void HandleGarbageCollect(HandleManager* man)
{
}

//-----------------------------------//

ReferenceCounted* HandleFind(HandleManager* man, HandleId id)
{
	return hash::get<ReferenceCounted*>(man->handles, (uint64)id, nullptr);
}

//-----------------------------------//

NAMESPACE_EXTERN_END