/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/containers/HashMap.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Object;

typedef uint32 HandleId;
const HandleId HandleInvalid = 0;

typedef HandleId  (*HandleCreateFn)(ReferenceCounted*);
typedef void      (*HandleDestroyFn)(HandleId id);
typedef ReferenceCounted* (*HandleResolveFn)(HandleId id);

typedef HashMap<HandleId, ReferenceCounted*> HandleMap;

struct API_CORE HandleManager
{
    HandleMap handles;
    Atomic<uint32> nextHandle;
};

API_CORE HandleManager*    HandleCreateManager( Allocator* );
API_CORE void              HandleDestroyManager( HandleManager* );
API_CORE HandleId          HandleCreate(HandleManager*, ReferenceCounted*);
API_CORE void              HandleDestroy(HandleManager*, HandleId id);  
API_CORE ReferenceCounted* HandleFind(HandleManager*, HandleId id);
API_CORE void              HandleGarbageCollect(HandleManager*);

typedef scoped_ptr<HandleManager, HandleDestroyManager> HandleManagerPtr;
#define pHandleCreateManager(alloc, ...) CreateScopedPtr(HandleCreateManager, \
    alloc, __VA_ARGS__)

//-----------------------------------//

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
        if(add_ref) addReference();
    }

    Handle(Handle const & rhs): id(rhs.id)
    {
        addReference();
    }

    ~Handle()
    {
        removeReference();
    }

    Handle& operator=(const Handle& rhs)
    {
        setId(rhs.id);
        return *this;
    }

#ifdef COMPILER_SUPPORTS_CXX11
    Handle(Handle && rhs) : id(HandleInvalid)
    {
        setId(rhs.id);
        rhs.reset();
    }

    Handle& operator=(Handle && rhs)
    {
        if(this != &rhs)
        {
            setId(rhs.id);
            rhs.reset();
        }

        return *this;
    }
#endif

    T& operator*() const
    {
        T* px = Resolve();
        return *px;
    }

    T* operator->() const
    {
        T* px = Resolve();
        return px;
    }

    void addReference()
    {
        if(id == HandleInvalid) return;
        
        T* px = Resolve();
        if(px) px->addReference();
    }

    void removeReference()
    {
        if(id == HandleInvalid) return;

        T* px = Resolve();
        
        if(px && px->releaseReference())
        {
            if(DFn) DFn(id);
            Deallocate(px);
        }
    }

    inline void reset()
    {
        setId(HandleInvalid);
    }

    void Destroy()
    {
        T* px = Resolve();
        
        if(px)
        {
            if(DFn) DFn(id);
            Deallocate(px);
        }
        
        id = HandleInvalid;
    }

    T* Resolve() const
    {
        if( !RFn ) return nullptr;

        T* px = (T*) RFn(id);
        return px;
    }

    operator bool() const
    {
        return (id != HandleInvalid) && Resolve();
    }

    inline HandleId getId() const { return id; }

    void setId(const HandleId& newId)
    {
        T* old = Resolve();

        if(old && old->releaseReference())
        {
            if(DFn) DFn(id);
            Deallocate(old);
        }

        id = newId;
        addReference();
    }

    unsigned ToHash() const
    {
        return id;
    }

    HandleId id;
};

template<typename T, typename U, HandleResolveFn RFn, HandleDestroyFn DFn>
Handle<T, RFn, DFn> HandleCast(const Handle<U, RFn, DFn>& hn)
{
    return Handle<T, RFn, DFn>( hn.getId() );
}

//-----------------------------------//

NAMESPACE_CORE_END

#define FWD_DECL_HANDLE(T) \
    NAMESPACE_CORE_BEGIN \
        class T; \
        typedef Handle<T> T##Handle; \
    NAMESPACE_CORE_END
