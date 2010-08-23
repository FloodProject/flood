/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

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

class VAPOR_API Type : private boost::noncopyable
{
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
		return this == &(T::getType());
	}

	// Returns if this type inherits from the given type.
	template<typename T> bool inherits() const
	{
		if( parent == nullptr )
			return false;

		if( parent == &(T::getType()) )
			return true;
		else
			return parent->inherits<T>();
	}

protected:

	Type(MetaType::Enum type, const std::string& name);
	Type(MetaType::Enum type, const std::string& name, const Type& parent);

	// Name of the type.
	const std::string name;

	// Parent of the type.
	const Type* parent;

	// Meta type of the type.
	MetaType::Enum metaType;
};

//-----------------------------------//

typedef std::map<std::string, const Type*> TypeRegistryMap;

/**
 * Stores a globally acessible mapping of type names and their instances.
 * This can be useful when you need to get information about a type just
 * by its name. Things like serialization need this information.
 */

class TypeRegistry
{
public:

	// Registers a new type mapping.
	void registerType(const Type& type);

	// Gets a type given a name.
	const Type* getType(const std::string& type);

	// Maps all the type names to their type instances.
	TypeRegistryMap registeredTypes;

	static TypeRegistry& getInstance();
};

//-----------------------------------//

} // end namespace
