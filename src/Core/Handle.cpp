/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Handle.h"
#include "Core/Log.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

HandleManager* HandleCreateManager( Allocator* alloc )
{
	HandleManager* man = Allocate(HandleManager, alloc);
	man->nextHandle = 0;
	return man;
}

//-----------------------------------//

void HandleDestroyManager( HandleManager* man )
{
	if( !man ) return;

	const HandleMap& handles = man->handles;

	if( handles.size() > 0 )
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
	handles[id] = ref;
	
	return id;
}

//-----------------------------------//

void HandleDestroy(HandleManager* man, HandleId id)
{
	if( !man ) return;
	
	HandleMap& handles = man->handles;
	HandleMap::iterator it = handles.find(id);

	if( it == handles.end() )
		return;

	handles.erase(it);
}

//-----------------------------------//

void HandleGarbageCollect(HandleManager* man)
{
}

//-----------------------------------//

ReferenceCounted* HandleFind(HandleManager* man, HandleId id)
{
	HandleMap& handles = man->handles;
	HandleMap::iterator it = handles.find(id);
	
	if( it == handles.end() )
		return nullptr;

	return handles[id];
}

//-----------------------------------//

NAMESPACE_EXTERN_END