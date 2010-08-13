/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/core/Primitive.h"

namespace vapor {

//-----------------------------------//

const Primitive Primitive::_bool(PrimitiveType::Bool, "bool");
const Primitive Primitive::_int(PrimitiveType::Integer, "int");
const Primitive Primitive::_uint(PrimitiveType::UnsignedInteger, "uint");
const Primitive Primitive::_float(PrimitiveType::Float, "float");
const Primitive Primitive::_double(PrimitiveType::Double, "double");
const Primitive Primitive::_string(PrimitiveType::String, "string");
const Primitive Primitive::_Vector3(PrimitiveType::Vector3, "Vector3");
const Primitive Primitive::_Color(PrimitiveType::Color, "Color");

//-----------------------------------//

Primitive::Primitive(PrimitiveType::Enum primitive, const std::string& name)
	: Type(MetaType::Primitive, name)
	, primitive(primitive)
{ }

//-----------------------------------//

bool Primitive::isBool() const
{
	return primitive == PrimitiveType::Bool;
}

//-----------------------------------//

bool Primitive::isInteger() const
{
	return primitive == PrimitiveType::Integer;
}

//-----------------------------------//

bool Primitive::isUnsignedInteger() const
{
	return primitive == PrimitiveType::UnsignedInteger;
}

//-----------------------------------//

bool Primitive::isFloat() const
{
	return primitive == PrimitiveType::Float;
}

//-----------------------------------//

bool Primitive::isDouble() const
{
	return primitive == PrimitiveType::Double;
}

//-----------------------------------//

bool Primitive::isString() const
{
	return primitive == PrimitiveType::String;
}

//-----------------------------------//

bool Primitive::isVector3() const
{
	return primitive == PrimitiveType::Vector3;
}

//-----------------------------------//

bool Primitive::isColor() const
{
	return primitive == PrimitiveType::Color;
}


//-----------------------------------//

} // end namespace