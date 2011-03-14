/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"
#include <functional>
#include <cassert>

namespace vapor {

//-----------------------------------//

/**
 * Inherit from this class to be able to use reference counting semantics.
 * Currently this is implemented using intrusive_ptr semantics.
 */

class CORE_API ReferenceCounted
{
public:

	int32 getReferenceCount() const { return AtomicRead(&references); }
	inline void addReference() { AtomicIncrement(&references); }
	inline bool releaseReference() { return AtomicDecrement(&references) == 0; }

protected:
	
	ReferenceCounted() : references(0) {}

#ifdef SWIG
	mutable volatile Atomic references;
#else
	mutable volatile VAPOR_ALIGN_BEGIN(32) Atomic references VAPOR_ALIGN_END(32);
#endif
};

template<typename T> class RefPtr
{
public:

    RefPtr(): px(nullptr) { }

    RefPtr(T* p, bool add_ref = true) : px(p)
    {
        if( px != nullptr && add_ref )
			px->addReference();
    }

	template<typename U>
    RefPtr( RefPtr<U> const & rhs ) : px( rhs.get() )
    {
        if( px != nullptr )
			px->addReference();
    }

    RefPtr(RefPtr const & rhs): px(rhs.px)
    {
        if( px != nullptr )
			px->addReference();
    }

    ~RefPtr()
    {
        if( px != nullptr && px->releaseReference() )
			delete px;
    }

    template<typename U>
	RefPtr& operator=(const RefPtr<U>& rhs)
    {
        RefPtr(rhs).swap(*this);
        return *this;
    }

#ifdef VAPOR_COMPILER_MSVC_2010
    RefPtr(RefPtr && rhs): px( rhs.px )
    {
        rhs.px = 0;
    }

    RefPtr & operator=(RefPtr && rhs)
    {
        RefPtr( static_cast< RefPtr && >( rhs ) ).swap(*this);
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

private:

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

} // end namespace

#define TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( type ) \
	typedef RefPtr<type> type##Ptr;

#define FWD_DECL_INTRUSIVE(T)						\
	namespace vapor {								\
		class T;									\
		TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( T );	\
	} // end namespace