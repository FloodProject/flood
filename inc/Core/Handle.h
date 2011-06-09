/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

typedef uint32 HandleId;
const HandleId HandleInvalid = 0;

typedef void* (HandleResolveFn)(HandleId id);
typedef void  (HandleDestroyFn)(HandleId id);

typedef std::map<HandleId, void*> HandleMap;

struct HandleManager
{
	HandleMap handles;
	volatile Atomic nextHandle;
};

HandleManager*     HandleCreateManager( Allocator* );
void               HandleDestroyManager( HandleManager* );
API_CORE HandleId  HandleCreate(HandleManager*, void*);
API_CORE void      HandleDestroy(HandleManager*, HandleId id);	
API_CORE void*     HandleFind(HandleManager*, HandleId id);
API_CORE void      HandleGarbageCollect(HandleManager*);

typedef scoped_ptr<HandleManager, HandleDestroyManager> HandleManagerPtr;
#define pHandleCreateManager(alloc, ...) CreateScopedPtr(HandleCreateManager, alloc, __VA_ARGS__)

//-----------------------------------//

EXTERN_END

#ifdef COMPILER_MSVC
#pragma warning(disable: 4550)
#endif

template<typename T, HandleResolveFn RFn, HandleDestroyFn DFn>
class Handle
{
public:

	Handle() : id(HandleInvalid) { }

    Handle(HandleId id, bool add_ref = true) : id(id)
    {
        if( id == HandleInvalid )
			return;
		
		T* px = Resolve();
		if(px) px->addReference();
    }

    Handle(Handle const & rhs): id(rhs.id)
    {
        if( id == HandleInvalid )
			return;
		
		T* px = Resolve();
		if(px) px->addReference();
    }

    ~Handle()
    {
        if( id == HandleInvalid )
			return;
		
		T* px = Resolve();
		
		if(px && px->releaseReference() && DFn)
			DFn(id);
    }

    Handle& operator=(const Handle& rhs)
    {
        id = rhs.id;

		T* px = Resolve();
		if(px) px->addReference();

        return *this;
    }

#ifdef COMPILER_MSVC_2010
    Handle(Handle && rhs): id(rhs.id)
    {
        rhs.id = HandleInvalid;
    }

    Handle& operator=(Handle && rhs)
    {
		if(this != &rhs)
		{
			id = rhs.id;
			rhs.id = HandleInvalid;
		}

        return *this;
    }
#endif

	void Destroy( Allocator* alloc )
	{
		T* px = Resolve();
		
		if(px)
		{
			if(DFn) DFn(id);
			Deallocate<T>(px);
		}
		
		id = HandleInvalid;
	}

    T* Resolve() const
    {
		T* px = (T*) RFn(id);
        return px;
    }

    operator bool() const
    {
        return (id != HandleInvalid) && (Resolve() != nullptr);
    }

    HandleId id;
};

template<typename T, typename U, HandleResolveFn RFn, HandleDestroyFn DFn>
Handle<T, RFn, DFn> HandleCast(const Handle<U, RFn, DFn>& hn)
{
    return Handle<T, RFn, DFn>(hn.id);
}

//-----------------------------------//

NAMESPACE_END