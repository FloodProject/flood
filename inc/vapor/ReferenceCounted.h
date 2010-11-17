/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Atomic.h"

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

	GETTER(ReferenceCount, uint, references.get())

protected:
	
	ReferenceCounted() 
		: references(0) 
	{ }

	virtual ~ReferenceCounted()
	{ }

private:

	mutable Atomic references;

	friend void intrusive_ptr_add_ref(ReferenceCounted* const ptr);
	friend void intrusive_ptr_release(ReferenceCounted* const ptr);
};

//-----------------------------------//

inline void intrusive_ptr_add_ref(ReferenceCounted* const ptr)
{
	ptr->references.inc();
}

//-----------------------------------//

inline void intrusive_ptr_release(ReferenceCounted* const ptr)
{
	// Delete object when reference count reaches 0.
	if(ptr->references.dec() == 0)
		delete ptr;
}

//-----------------------------------//

} // end namespace