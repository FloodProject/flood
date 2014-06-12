/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Handle.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

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

    if( handles.Size() > 0 )
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
    
    HandleId id = man->nextHandle.increment();
    handles[id] = ref;
    
    return id;
}

//-----------------------------------//

void HandleDestroy(HandleManager* man, HandleId id)
{
    if( !man ) return;
    
    HandleMap& handles = man->handles;
    HandleMap::Iterator it = handles.Find(id);

    if( it == handles.End() )
        return;

    handles.Erase(it);
}

//-----------------------------------//

void HandleGarbageCollect(HandleManager* man)
{
}

//-----------------------------------//

ReferenceCounted* HandleFind(HandleManager* man, HandleId id)
{
    HandleMap& handles = man->handles;
    HandleMap::Iterator it = handles.Find(id);
    
    if (it == handles.End())
        return nullptr;

    return handles[id];
}

//-----------------------------------//

NAMESPACE_CORE_END