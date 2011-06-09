/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"

NAMESPACE_BEGIN

//-----------------------------------//

struct Class;

/**
 * Objects are the root class for types using the reflection services.
 */

REFLECT_DECLARE_CLASS(Object)

struct NO_VTABLE Object : public ReferenceCounted
{
	// Gets the type of the object.
	API_CORE virtual Class* getType() const { return nullptr; }

	// Serialization fix-up phase.
	API_CORE virtual void fixUp() {}

	// Field change notifications.
	API_CORE virtual void onFieldChanged(const Field& field) { }

	// Returns if this type is the same as the given type.
	template<typename T> bool is()
	{
		return ReflectionIsEqual(getType(), T::getStaticType());
	}

	// Returns if this type inherits from the given type.
	template<typename T> bool inherits()
	{
		return ClassInherits(getType(), T::getStaticType());
	}

	virtual ~Object() {}

protected:

	Object() {}
};

#define REFLECT_DECLARE_OBJECT(className)                                            \
	public:                                                                          \
	virtual Class* getType() const OVERRIDE { return ReflectionGetType(className); } \
	static Class* getStaticType() { return ReflectionGetType(className); }           \
	REFLECT_DECLARE_CLASS_FRIEND(className)

//-----------------------------------//

NAMESPACE_END
