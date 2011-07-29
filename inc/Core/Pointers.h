/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Memory.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// scoped_ptr mimics a built-in pointer except that it guarantees deletion
// of the object pointed to, either on destruction of the scoped_ptr or via
// an explicit reset(). scoped_ptr is based on Boost's scoped_ptr but extends
// it with custom deallocators.

template<typename T, void (*Destroy)(T*)>
class scoped_ptr // noncopyable
{
    typedef scoped_ptr<T, Destroy> this_type;

public:

    typedef T element_type;

    explicit scoped_ptr( T* p ) : px(p)
    { }

    ~scoped_ptr() // never throws
    {
		Destroy(px);
    }

    void reset(T * p = 0) // never throws
    {
        assert( p == 0 || p != px ); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator*() const // never throws
    {
        assert( px != 0 );
        return *px;
    }

    T * operator->() const // never throws
    {
        assert( px != 0 );
        return px;
    }

    T * get() const // never throws
    {
        return px;
    }
    
    operator bool() const // never throws
    {
        return px != nullptr;
    }

	operator T*() const
	{
		return px;
	}

	void swap(scoped_ptr & b) // never throws
    {
        T * tmp = b.px;
        b.px = px;
        px = tmp;
    }

private:

    T* px;

    scoped_ptr(scoped_ptr const &);
    scoped_ptr & operator=(scoped_ptr const &);

    void operator==( scoped_ptr const& ) const;
    void operator!=( scoped_ptr const& ) const;
};

#if 0
template<class T> inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) // never throws
{
    a.swap(b);
}

// get_pointer(p) is a generic way to say p.get()

template<class T> inline T * get_pointer(scoped_ptr<T> const & p)
{
    return p.get();
}
#endif

#define CreateScopedPtr(Create, Alloc, ...) Create(Alloc, __VA_ARGS__)

//-----------------------------------//

NAMESPACE_CORE_END
