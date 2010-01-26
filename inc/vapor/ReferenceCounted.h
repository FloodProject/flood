/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
#include "vapor/Platform.h"

namespace vapor {

//-----------------------------------//

/**
 * Inherit from this class to be able to use reference counting semantics.
 * Currently this is implemented using intrusive_ptr's which are faster but
 * also less full featured than the better known shared_ptr's (no weak_ptr).
 * Please beware that this is not thread-safe.
 */

class VAPOR_API ReferenceCounted
{
protected:
	
	ReferenceCounted() : references( 0 ) { }
	virtual ~ReferenceCounted() { }

	int getReferenceCount() { return references; }

private:

	int references;

	friend void intrusive_ptr_add_ref(ReferenceCounted* ptr);
	friend void intrusive_ptr_release(ReferenceCounted* ptr);
};

//-----------------------------------//

inline void intrusive_ptr_add_ref(ReferenceCounted* ptr)
{
	// increment reference count of object *p
	++(ptr->references);
}

//-----------------------------------//

inline void intrusive_ptr_release(ReferenceCounted* ptr)
{
	// decrement reference count
	// delete object when reference count reaches 0
	if (--(ptr->references) == 0)
		delete ptr;
}

//-----------------------------------//

} // end namespace