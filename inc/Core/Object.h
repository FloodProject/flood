/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/Reflection.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class Class;

/**
 * Objects are the root class for types using the reflection services.
 */

API_CORE REFLECT_DECLARE_CLASS(Object)

struct API_CORE NO_VTABLE Object : public ReferenceCounted
{
	// Gets the type of the object.
	virtual Class* getType() const { return nullptr; }

	// Serialization fix-up phase.
	virtual void fixUp() {}

	// Field change notifications.
	virtual void onFieldChanged(const Field& field) { }

	// Returns if this type is the same as the given type.
	template<typename T> bool is()
	{
		return ReflectionIsEqual(getType(), T::getStaticType());
	}

	// Returns if this type inherits from the given type.
	template<typename T> bool inherits()
	{
		return getType()->inherits(T::getStaticType());
	}

	virtual ~Object() {}

protected:

	Object() {}
};

#define REFLECT_DECLARE_STATIC_CLASS(className) \
	public: \
	static Class* getStaticType() { return className##GetType(); }

#define REFLECT_DECLARE_OBJECT(className) \
	public: \
	virtual Class* getType() const OVERRIDE { return className##GetType(); } \
	REFLECT_DECLARE_STATIC_CLASS(className) \
	REFLECT_DECLARE_CLASS_FRIEND(className)

//-----------------------------------//

NAMESPACE_CORE_END
