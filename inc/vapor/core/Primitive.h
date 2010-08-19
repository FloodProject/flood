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
 * Describes the type of the primitive type.
 */

namespace PrimitiveType
{
	enum Enum
	{
		Bool,
		Integer,
		UnsignedInteger,
		Float,
		Double,
		String,
		Vector3,
		Color,
	};
}

/**
 * This class provides the type system with information about the
 * primitive C++ language types, like booleans, integers and strings.
 */

class VAPOR_API Primitive : public Type
{
public:

	Primitive(PrimitiveType::Enum, const std::string& name);

	// Gets if this type represents a bool.
	bool isBool() const;

	// Gets if this type represents an integer.
	bool isInteger() const;

	// Gets if this type represents an unsigned integer.
	bool isUnsignedInteger() const;

	// Gets if this type represents a float.
	bool isFloat() const;

	// Gets if this type represents a double.
	bool isDouble() const;

	// Gets if this type represents an string.
	bool isString() const;

	// Gets if this type represents a vector.
	bool isVector3() const;

	// Gets if this type represents a color.
	bool isColor() const;

protected:

	// Primitive type of the type.
	PrimitiveType::Enum primitive;

public:

	// Static primitive types.
	static const Primitive _bool;
	static const Primitive _int;
	static const Primitive _uint;
	static const Primitive _float;
	static const Primitive _double;
	static const Primitive _string;
	static const Primitive _Vector3;
	static const Primitive _Color;
};



//-----------------------------------//

} // end namespace
