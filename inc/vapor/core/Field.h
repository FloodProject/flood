/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "core/Type.h"

namespace vapor {

//-----------------------------------//

/**
 * Represents a field in a class.
 */

class VAPOR_API Field
{
	DECLARE_UNCOPYABLE(Field)

public:

	Field( const Type& type );

	// Gets the value of the field in the object.
	template<typename T>
	const T& get( const void* object ) const
	{
		T* ptr = (T*)((byte*)object + offset);
		return *ptr;
	}

	// Sets the value of the field in the object.
	template<typename T>
	void set( const void* object, const T& value ) const
	{
		T* ptr = (T*)((byte*)object + offset);
		*ptr = value;
	}

	// Returns if the field is a pointer.
	bool isPointer() const;

public:

	bool pointer;
	short offset;
	short size;
	std::string name;
	const Type& type;
};

//-----------------------------------//

} // end namespace
