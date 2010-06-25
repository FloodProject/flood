/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Inherit from this class to be able to use reference counting semantics.
 * Currently this is implemented using intrusive_ptr from boost which are
 * faster but have less features than shared_ptr (no weak references).
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

	atomic_int references;

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