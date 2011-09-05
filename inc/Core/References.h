/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct ReferenceCounted;

API_CORE int32 ReferenceGetCount(ReferenceCounted*);
API_CORE void ReferenceAdd(ReferenceCounted*);
API_CORE bool ReferenceRelease(ReferenceCounted*);

struct API_CORE NO_VTABLE ReferenceCounted
{	
	ReferenceCounted() : references(0) {}
	ReferenceCounted(const ReferenceCounted&) : references(0) {}
	ReferenceCounted& operator=(const ReferenceCounted&) { return *this; }
	
	inline void addReference() { ReferenceAdd(this); }
	inline bool releaseReference() { return ReferenceRelease(this); }

	volatile Atomic references;
};

//-----------------------------------//

EXTERN_END

template<typename T> class RefPtr
{
public:

    RefPtr(): px(nullptr) { }

    RefPtr(T* p, bool add_ref = true) : px(p)
    {
        if( px != nullptr && add_ref ) px->addReference();
    }

	template<typename U>
    RefPtr( RefPtr<U> const & rhs ) : px( rhs.get() )
    {
        if( px != nullptr ) px->addReference();
    }

    RefPtr(RefPtr const & rhs): px(rhs.px)
    {
        if( px != nullptr ) px->addReference();
    }

    ~RefPtr()
    {
        if( px != nullptr && px->releaseReference() )
            Deallocate(px);
    }

    template<typename U>
	RefPtr& operator=(const RefPtr<U>& rhs)
    {
        RefPtr(rhs).swap(*this);
        return *this;
    }

#ifdef COMPILER_MSVC_2010
    RefPtr(RefPtr&& rhs): px(rhs.px)
    {
        rhs.px = nullptr;
    }

    RefPtr& operator=(RefPtr&& rhs)
    {
        RefPtr(static_cast<RefPtr&&>(rhs)).swap(*this);
        return *this;
    }
#endif

    RefPtr& operator=(const RefPtr& rhs)
    {
        RefPtr(rhs).swap(*this);
        return *this;
    }

    RefPtr& operator=(T* rhs)
    {
        RefPtr(rhs).swap(*this);
        return *this;
    }

    void reset()
    {
        RefPtr().swap(*this);
    }

    void reset( T* rhs )
    {
        RefPtr(rhs).swap(*this);
    }

    T* get() const
    {
        return px;
    }

    T& operator*() const
    {
        assert( px != nullptr );
        return *px;
    }

    T* operator->() const
    {
        assert( px != nullptr );
        return px;
    }

    typedef T* RefPtr::*unspecified_bool_type;

    operator unspecified_bool_type() const // never throws
    {
        return px == nullptr ? false : &RefPtr::px;
    }

    void swap(RefPtr& rhs)
    {
        T* tmp = px;
        px = rhs.px;
        rhs.px = tmp;
    }

public:

    T* px;
};

template<class T, class U> inline bool operator==(const RefPtr<T>& a, const RefPtr<U>& b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(const RefPtr<T>& a, const RefPtr<U>& b)
{
    return a.get() != b.get();
}

template<class T, class U> inline bool operator==(const RefPtr<T>& a, U* b)
{
    return a.get() == b;
}

template<class T, class U> inline bool operator!=(const RefPtr<T>& a, U* b)
{
    return a.get() != b;
}

template<class T, class U> inline bool operator==(T* a, const RefPtr<U>& b)
{
    return a == b.get();
}

template<class T, class U> inline bool operator!=(T* a, const RefPtr<U>& b)
{
    return a != b.get();
}

template<class T> inline bool operator<(const RefPtr<T>& a, const RefPtr<T>& b)
{
    return std::less<T *>()(a.get(), b.get());
}

template<class T> void swap(const RefPtr<T>& lhs, const RefPtr<T>& rhs)
{
    lhs.swap(rhs);
}

template<class T, class U> RefPtr<T> RefCast(const RefPtr<U>& p)
{
    return static_cast<T*>(p.get());
}

//-----------------------------------//

NAMESPACE_CORE_END

#define TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( type ) \
	typedef RefPtr<type> type##Ptr;

#define FWD_DECL_INTRUSIVE(T)						\
	NAMESPACE_CORE_BEGIN							\
		class T;									\
		TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( T );	\
	NAMESPACE_CORE_END