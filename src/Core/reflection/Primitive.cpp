/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#if 0

#include "Core/API.h"
#include "Core/Primitive.h"
#include "Math/Vector.h"
#include "Math/Color.h"
#include "Math/Quaternion.h"

namespace vapor {

//-----------------------------------//

#define PT(type) PrimitiveType::##T

const Primitive Primitive::_bool(PrimitiveType::Bool, "bool", sizeof(bool));
const Primitive Primitive::_int(PrimitiveType::Integer, "int", sizeof(int));
const Primitive Primitive::_uint(PrimitiveType::UnsignedInteger, "uint", sizeof(uint));
const Primitive Primitive::_float(PrimitiveType::Float, "float", sizeof(float));
const Primitive Primitive::_string(PrimitiveType::String, "string", sizeof(String));
const Primitive Primitive::_Vector3(PrimitiveType::Vector3, "Vector3", sizeof(Vector3));
const Primitive Primitive::_Color(PrimitiveType::Color, "Color", sizeof(Color));
const Primitive Primitive::_Quaternion(PrimitiveType::Quaternion, "Quaternion", sizeof(Quaternion));
const Primitive Primitive::_Bitfield(PrimitiveType::Bitfield, "Bitfield", sizeof(uint));

//-----------------------------------//

Primitive::Primitive(PrimitiveType::Enum primitive, const String& name, int size)
	: Type(MetaType::Primitive, name, size)
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

bool Primitive::isQuaternion() const
{
	return primitive == PrimitiveType::Quaternion;
}

//-----------------------------------//

bool Primitive::isColor() const
{
	return primitive == PrimitiveType::Color;
}

//-----------------------------------//

bool Primitive::isBitfield() const
{
	return primitive == PrimitiveType::Bitfield;
}

//-----------------------------------//

} // end namespace

#endif