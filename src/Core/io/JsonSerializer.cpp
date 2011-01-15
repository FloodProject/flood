/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "io/JsonSerializer.h"

#ifdef VAPOR_SERIALIZATION_JSON

namespace vapor {

//-----------------------------------//

static Vector3 convertValueToVector3( const Json::Value& value )
{
	Vector3 vec(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()) );

	return vec;
}

//-----------------------------------//

static Color convertValueToColor( const Json::Value& value )
{
	Color color(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()),
		float(value[3u].asDouble()) );

	return color;
}

//-----------------------------------//

static Quaternion convertValueToQuaternion( const Json::Value& value )
{
	Quaternion quat(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()),
		float(value[3u].asDouble()) );

	return quat;
}

//-----------------------------------//

static EulerAngles convertValueToEulerAngles( const Json::Value& value )
{
	EulerAngles ang(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()));

	return ang;
}

//-----------------------------------//

static Json::Value convertVector3( const Vector3& vec )
{
	Json::Value v;
	
	v.append(vec.x);
	v.append(vec.y);
	v.append(vec.z);
	
	return v;
}

//-----------------------------------//

static Json::Value convertColor( const Color& color )
{
	Json::Value v;
	
	v.append(color.r);
	v.append(color.g);
	v.append(color.b);
	v.append(color.a);
	
	return v;
}

//-----------------------------------//

static Json::Value convertQuaternion( const Quaternion& quat )
{
	Json::Value v;
	
	v.append(quat.x);
	v.append(quat.y);
	v.append(quat.z);
	v.append(quat.w);
	
	return v;
}

//-----------------------------------//

JsonSerializer::JsonSerializer(Stream& stream)
	: stream(stream)
{
	values.push(rootValue);
}

//-----------------------------------//

void JsonSerializer::flushStream()
{
	std::string text = values.top().toStyledString();
	stream.write(text);
}

//-----------------------------------//

void JsonSerializer::beginClass(const Class& type)
{
	const std::string& name = type.getName();
	
	Json::Value& top = values.top();
	values.push( top[name] );
}

//-----------------------------------//

void JsonSerializer::endClass()
{
	values.pop();
}

//-----------------------------------//

void JsonSerializer::beginArray(const Type& type)
{
	//const std::string& name = type.getName();
	//
	//Json::Value& top = values.top();
	//values.push( top[name] );
}

//-----------------------------------//

void JsonSerializer::endArray()
{
	//values.pop();
}

//-----------------------------------//

void JsonSerializer::writeField( const Field& field )
{
	Json::Value& top = values.top();
	values.push( top[field.name] );
}

//-----------------------------------//

void JsonSerializer::writeBool( const Primitive& prim, bool value )
{
	Json::Value& top = values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::writeInt( const Primitive& prim, int value )
{
	Json::Value& top = values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::writeUInt( const Primitive& prim, uint value )
{
	Json::Value& top = values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::writeFloat( const Primitive& prim, float value )
{
	Json::Value& top = values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::writeDouble( const Primitive& prim, double value )
{
	Json::Value& top = values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::writeString( const Primitive& prim, const std::string& value )
{
	Json::Value& top = values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::writeVector3( const Primitive& prim, const Vector3& value )
{
	Json::Value& top = values.top();
	top = convertVector3(value);
}

//-----------------------------------//

void JsonSerializer::writeQuaternion( const Primitive& prim, const Quaternion& value )
{
	Json::Value& top = values.top();
	top = convertQuaternion(value);
}

//-----------------------------------//

void JsonSerializer::writeColor( const Primitive& prim, const Color& value )
{
	Json::Value& top = values.top();
	top = convertColor(value);
}

//-----------------------------------//

void JsonSerializer::writeBitfield( const Primitive& prim, const std::bitset<32>& value )
{
	Json::Value& top = values.top();
	top = value.to_string();
}

//-----------------------------------//

//void JsonSerializer::writeHandle( const Handle& )
//{ }

//-----------------------------------//

//void Serializer::setFieldFromValue( const Field& field, void* object, const Json::Value& value )
//{
//	if( !field.type.isPrimitive() )
//	{
//		Log::debug( "field: %s", field.name.c_str() );
//		return;
//	}
//
//	//assert( field.type.isPrimitive() );
//	const Primitive& type = (const Primitive&) field.type;
//
//	if( type.isBool() )
//	{
//		bool val = value.asBool();
//		field.set<bool>(object, val);
//	}
//	//-----------------------------------//
//	else if( type.isInteger() )
//	{
//		int val = value.asInt();
//		field.set<int>(object, val);
//	}
//	//-----------------------------------//
//	else if( type.isFloat() )
//	{
//		double val = value.asDouble();
//		field.set<float>(object, float(val));
//	}
//	//-----------------------------------//
//	else if( type.isString() )
//	{
//		std::string val = value.asString();
//		field.set<std::string>(object, val);
//	}
//	//-----------------------------------//
//	else if( type.isColor() )
//	{
//		Color val = convertValueToColor(value);
//		field.set<Color>(object, val);
//	}
//	//-----------------------------------//
//	else if( type.isVector3() )
//	{
//		Vector3 val = convertValueToVector3(value);
//		field.set<Vector3>(object, val);
//	}
//	//-----------------------------------//
//	else if( type.isQuaternion() )
//	{
//		Quaternion val = convertValueToQuaternion(value);
//		field.set<Quaternion>(object, val);
//	}
//	//-----------------------------------//
//	else if( type.isBitfield() )
//	{
//		std::string str = value.asString();
//		std::bitset<32> val(str);
//		field.set< std::bitset<32> >(object, val);
//	}
//	else
//	//-----------------------------------//
//		assert( false );
//}

////-----------------------------------//
//
//Json::Value Serializer::valueFromEnum( const Field& field, void* object )
//{
//	assert( field.type.isEnum() );
//	const Enum& type = (const Enum&) field.type;
//
//	Json::Value v;
//
//	//foreach( const EnumValuesPair& p, type.getValues() )
//	//{
//	//	arrValues.Add( p.first, p.second );
//	//}
//
//	return v;
//}

//-----------------------------------//

} // end namespace

#endif