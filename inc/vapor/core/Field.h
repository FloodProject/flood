/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Type.h"

namespace vapor {

//-----------------------------------//

namespace Qualifier
{
	enum Enum
	{
		Array			= 1 << 0,
		Map				= 1 << 1,
		Set				= 1 << 2,
		Handle			= 1 << 3,
		RawPointer		= 1 << 4,
		SharedPointer	= 1 << 5,
		RefPointer		= 1 << 6,
		ReadOnly		= 1 << 7,
	};
}

//-----------------------------------//

typedef void (*SetterFunctionPtr)(void*, void*);

/**
 * Represents a field in a class.
 */

class CORE_API Field
{
	DECLARE_UNCOPYABLE(Field)
	friend class Class;

public:

	Field( const Type& type );

	// Sets the qualifier of the field.
	void setQualifier( byte qualifier );

	// Sets the setter function of the field.
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

	// Returns if the field is an array.
	bool isArray() const;

	// Returns if the field is read-only.
	bool isReadOnly() const;

	// Type of the field.
	const Type& type;

	// Name of the field in the class.
	std::string name;

	// Qualifiers of the field.
	byte qualifiers;

	// Offset in the class.
	short offset;

	// Size of the field.
	short size;

	// Size of the pointer.
	short pointerSize;

	// Setter function.
	SetterFunctionPtr setterFunction;
};

//-----------------------------------//

} // end namespace
