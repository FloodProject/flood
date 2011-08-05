/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION

NAMESPACE_CORE_BEGIN

//-----------------------------------//

template<typename T>
static void PointerSetObject( const Field* field, void* address, T* object )
{
	if( FieldIsRawPointer(field) )
	{
		T** raw = (T**) address;
		*raw = object;
	}
	else if( FieldIsRefPointer(field) )
	{
		T* ref = (T*) object;
		RefPtr<T>* ref_obj = (RefPtr<T>*) address;
		ref_obj->reset(ref);
	}
	else if( FieldIsHandle(field) )
	{
		assert(0 && "Not implemented");
	}
	else
	{
		assert(0 && "Not implemented");
		//memcpy(element, object, size);
	}
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif