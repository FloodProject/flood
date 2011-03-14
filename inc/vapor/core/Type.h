/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Registry.h"

namespace vapor {

//-----------------------------------//

/**
 * Describes the type of the type.
 */

namespace MetaType
{
	enum Enum
	{
		Primitive,
		Structure,
		Class,
		Enumeration
	};
}

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking, reflection
 * (introspection) and serialization.
 */

class CORE_API VAPOR_PURE Type
{
	DECLARE_UNCOPYABLE(Type)

public:

	virtual ~Type();

	// Gets if this type represents a primitive.
	bool isPrimitive() const;

	// Gets if this type represents a struct.
	bool isStruct() const;

	// Gets if this type represents a class.
	bool isClass() const;

	// Gets if this type represents an enum.
	bool isEnum() const;

	// Returns if this type is the given type.
	bool is(const Type& type) const;

	// Returns if this type inherits from the given type.
	bool inherits(const Type& type) const;

	// Returns if this type is the same as the given type.
	template<typename T> bool is() const
	{
		return is(T::getStaticType());
	}

	// Returns if this type inherits from the given type.
	template<typename T> bool inherits() const
	{
		return inherits(T::getStaticType());
	}

	// Meta type of the type.
	MetaType::Enum type;

	// Name of the type.
	const String name;

	// Parent of the type.
	const Type* parent;

	// Size of the type.
	int size;

	// Gets the type registry.
	static Registry& GetRegistry();

protected:

	Type(MetaType::Enum type, const String& name, int size);
	Type(MetaType::Enum type, const String& name, const Type& parent, int size);

	// Performs initialization.
	void init();
};

//-----------------------------------//

} // end namespace
