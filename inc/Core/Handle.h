/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

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
void               HandleDestroyManager( HandleManager*, Allocator* );
API_CORE HandleId  HandleCreate(HandleManager*, void*);
API_CORE void      HandleDestroy(HandleManager*, HandleId id);	
API_CORE void*     HandleFind(HandleManager*, HandleId id);
API_CORE void      HandleGarbageCollect(HandleManager*);

typedef scoped_ptr<HandleManager, HandleDestroyManager> HandleManagerPtr;
#define pHandleCreateManager(alloc, ...) CreateScopedPtr(HandleCreateManager, alloc, __VA_ARGS__)

//-----------------------------------//

EXTERN_END

#ifdef VAPOR_COMPILER_MSVC
#pragma warning(disable: 4550)
#endif

template<typename T, HandleResolveFn RFn, HandleDestroyFn DFn = nullptr>
class Handle
{
public:

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
		if(px) px->releaseReference();
    }

    Handle& operator=(const Handle& rhs)
    {
        rhs.id = id
        return *this;
    }

	void Destroy( Allocator* alloc )
	{
		T* px = Resolve();
		
		if(px)
		{
			if(DFn) DFn(id);
			Deallocate<T>(alloc, px);
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

//-----------------------------------//

NAMESPACE_END