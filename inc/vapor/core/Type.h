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
		Struct,
		Class,
		Enumeration
	};
}

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking, reflection
 * (introspection) and serialization.
 */

class CORE_API Type
{
	DECLARE_UNCOPYABLE(Type)

public:

	virtual ~Type();

	// Gets the name of the type.
	const std::string& getName() const;

	// Gets the parent of the type.
	const Type* getParent() const;

	// Gets if this type represents a primitive.
	bool isPrimitive() const;

	// Gets if this type represents a struct.
	bool isStruct() const;

	// Gets if this type represents a class.
	bool isClass() const;

	// Gets if this type represents an enum.
	bool isEnum() const;

	// Returns if this type is the same as the given type.
	template<typename T> bool is() const
	{
		return this == &T::getStaticType();
	}

	// Returns if this type inherits from the given type.
	template<typename T> bool inherits() const
	{
		if( parent == nullptr )
			return false;

		return parent->is<T>() || parent->inherits<T>();
	}

	// Gets the type registry.
	static Registry& GetRegistry();

	// Meta type of the type.
	MetaType::Enum metaType;

	// Name of the type.
	const std::string name;

	// Parent of the type.
	const Type* parent;

	// Size of the type.
	int size;

protected:

	Type(MetaType::Enum type, const std::string& name, int size);
	Type(MetaType::Enum type, const std::string& name, const Type& parent, int size);

private:

	// Registers the type in the registry.
	void registerInstance();
};

//-----------------------------------//

} // end namespace
