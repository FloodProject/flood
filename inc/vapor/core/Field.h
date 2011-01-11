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

namespace Qualifier
{
	enum Enum
	{
		Handle	= 1 << 0,
		Pointer	= 1 << 1,
		Array	= 1 << 2,
		Map		= 1 << 3
	};
}

//-----------------------------------//

/**
 * Represents a field in a class.
 */

class CORE_API Field
{
	DECLARE_UNCOPYABLE(Field)

public:

	Field( const Type& type );

	// Sets the qualifier of the field.
	void setQualifier( byte qualifier );

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

	// Name of the field in the class.
	std::string name;

	// Type of the field.
	const Type& type;

	// Qualifiers of the field.
	byte qualifiers;

	// Offset in the class.
	short offset;

	// Size of the field.
	short size;
};

//-----------------------------------//

} // end namespace
