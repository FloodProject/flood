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

typedef void (*SetterFunctionPtr)(void*, void*);

class CORE_API Field
{
	DECLARE_UNCOPYABLE(Field)
	
	friend class Class;

public:

	Field( const Type& type );

	// Sets the qualifier of the field.
	void setQualifier( byte qualifier );

	// Sets the setter of the field.
	void setSetter( SetterFunctionPtr fn = nullptr );

	// Gets the value of the field in the object.
	template<typename T>
	const T& get( const void* object ) const
	{
		T* ptr = (T*)((byte*)object + offset);
		return *ptr;
	}

	// Sets the value of the field in the object.
	template<typename T>
	void set( const void* object, const T& value )
	{		
		if(setterFunction)
			setterFunction((void*) object, (void*) &value);
		else
			*(T*)((byte*)object + offset) = value;
	}

	// Returns if the field is a pointer.
	bool isPointer() const;

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

	// Setter function.
	SetterFunctionPtr setterFunction;
};

//-----------------------------------//

} // end namespace
