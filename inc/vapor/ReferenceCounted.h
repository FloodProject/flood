/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_THREADING
	#include <boost/atomic.hpp>
	typedef boost::atomic<int> atomic_int;
#else
	typedef int atomic_int;
#endif

namespace vapor {

//-----------------------------------//

/**
 * Inherit from this class to be able to use reference counting semantics.
 * Currently this is implemented using intrusive_ptr's which are faster but
 * also less full featured than the better known shared_ptr's (no weak_ptr).
 */

class VAPOR_API ReferenceCounted
{
public:

	GETTER(ReferenceCount, int, references)

protected:
	
	ReferenceCounted() 
		: references(0) 
	{ }

	virtual ~ReferenceCounted()
	{ }

private:

#if !defined(VAPOR_THREADING)
	int references;
#elif defined(VAPOR_THREADING) && defined(VAPOR_THREADING_BOOST)
	boost::atomic<int> references;
#elif defined(VAPOR_THREADING) && defined(VAPOR_THREADING_STD)
	std::atomic<int> references;
#else
	#error
#endif

	friend void intrusive_ptr_add_ref(ReferenceCounted* const ptr);
	friend void intrusive_ptr_release(ReferenceCounted* const ptr);
};

//-----------------------------------//

inline void intrusive_ptr_add_ref(ReferenceCounted* const ptr)
{
	// increment reference count of object
#if !defined(VAPOR_THREADING)
	++(ptr->references);
#else
	ptr->references.fetch_add(1);
#endif
}

//-----------------------------------//

inline void intrusive_ptr_release(ReferenceCounted* const ptr)
{
	// decrement reference count
	// delete object when reference count reaches 0
#if !defined(VAPOR_THREADING)
	if (--(ptr->references) == 0)
		delete ptr;
#else
	// fetch returns the old value, so should be 1.
	if(ptr->references.fetch_sub(1) == 1)
		delete ptr;
#endif
}

//-----------------------------------//

} // end namespace