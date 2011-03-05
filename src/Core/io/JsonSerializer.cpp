/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "io/JsonSerializer.h"
#include "Utilities.h"
#include "ReferenceCount.h"
#include "Log.h"

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
	values.push(&rootValue);
}

//-----------------------------------//

void JsonSerializer::processBegin(const ObjectData& data)
{ }

//-----------------------------------//

void JsonSerializer::processEnd(const ObjectData& data)
{
	// Always switch to the platform independent "C" locale when writing
	// JSON, else the library will format the data erroneously.

	LocaleSwitch locale;

	std::string text = rootValue.toStyledString();
	stream.write(text);
	stream.close();
}

//-----------------------------------//

void JsonSerializer::processClassBegin(const Class& type, bool parent)
{
	if(parent)
		return;

	Json::Value& top = *values.top();
	values.push( &top[type.name] );
}

//-----------------------------------//

void JsonSerializer::processClassEnd(const Class& type, bool parent)
{
	if(parent)
		return;
	
	values.pop();
}

//-----------------------------------//

void JsonSerializer::processFieldBegin( const Field& field )
{
	Json::Value& top = *values.top();
	values.push( &top[field.name] );
}

//-----------------------------------//

void JsonSerializer::processFieldEnd( const Field& field )
{
	values.pop();
}

//-----------------------------------//

void JsonSerializer::processArrayBegin(const Type& type, int size)
{

}

//-----------------------------------//

void JsonSerializer::processArrayEnd(const Type& type)
{

}

//-----------------------------------//

void JsonSerializer::processArrayElementBegin(int i)
{
	Json::Value& top = *values.top();
	values.push( &top[(uint)i] );
}

//-----------------------------------//

void JsonSerializer::processArrayElementEnd(int i)
{
	values.pop();
}

//-----------------------------------//

void JsonSerializer::processEnumBegin( const Enum& enuhm )
{

}

//-----------------------------------//

void JsonSerializer::processEnumEnd( const Enum& enuhm )
{

}

//-----------------------------------//
	
void JsonSerializer::processEnumElement(int value, const std::string& name)
{
	Json::Value& top = *values.top();
	top = name;
}

//-----------------------------------//

void JsonSerializer::processBool( const Primitive& prim, bool value )
{
	Json::Value& top = *values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::processInt( const Primitive& prim, int value )
{
	Json::Value& top = *values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::processUInt( const Primitive& prim, uint value )
{
	Json::Value& top = *values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::processFloat( const Primitive& prim, float value )
{
	Json::Value& top = *values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::processString( const Primitive& prim, const std::string& value )
{
	Json::Value& top = *values.top();
	top = value;
}

//-----------------------------------//

void JsonSerializer::processVector3( const Primitive& prim, const Vector3& value )
{
	Json::Value& top = *values.top();
	top = convertVector3(value);
}

//-----------------------------------//

void JsonSerializer::processQuaternion( const Primitive& prim, const Quaternion& value )
{
	Json::Value& top = *values.top();
	top = convertQuaternion(value);
}

//-----------------------------------//

void JsonSerializer::processColor( const Primitive& prim, const Color& value )
{
	Json::Value& top = *values.top();
	top = convertColor(value);
}

//-----------------------------------//

void JsonSerializer::processBitfield( const Primitive& prim, const uint& value )
{
	Json::Value& top = *values.top();
	top = value;
}

//----------------------------------------------------------------------//

JsonDeserializer::JsonDeserializer(Stream& stream)
	: stream(stream)
	, registry(Type::GetRegistry())
{ }

//-----------------------------------//

Object* JsonDeserializer::deserialize()
{
	std::string text;
	stream.read(text);
	stream.close();

	Json::Value rootValue;
	bool success = false;

	{
		LocaleSwitch locale;

		Json::Reader jsonReader;
		success = jsonReader.parse(text, rootValue, false);
	}

	if( !success )
		return nullptr;

	return processObject(rootValue);
}

//-----------------------------------//

Object* JsonDeserializer::processObject(const Json::Value& value)
{
	Object* object = nullptr;

	if( !value.isObject() )
		return nullptr;

	Json::Value::Members& members = value.getMemberNames();

	for( uint i = 0; i < members.size(); i++ )
	{
		const std::string& name = members[i];

		const Class* klass = (Class*) registry.getType(name);
		if( !klass ) continue;

		object = (Object*) klass->createInstance();
		
		if( !object ) continue;

		const Json::Value& fieldsValue = value[name];
		processFields(object, fieldsValue);

		object->fixUp();
	}

	return object;
}

//-----------------------------------//

void JsonDeserializer::processFields(Object* object, const Json::Value& value)
{
	if( !object ) return;

	const Class& type = (Class&) object->getType();

	Json::Value::Members& members = value.getMemberNames();

	for( uint i = 0; i < members.size(); i++ )
	{
		const std::string& name = members[i];

		Field* field = type.getField(name);
		if( !field ) continue;

		const Json::Value& fieldValue = value[name];
		if( fieldValue.isNull() ) continue;

		processField(object, *field, fieldValue);
	}
}

//-----------------------------------//

void JsonDeserializer::processField(Object* object, Field& field, const Json::Value& value)
{
	if( field.isArray() && value.isArray() )
		processArray(object, field, value);
	else if( field.type.isClass() )
		processObject(value);
	else if( field.type.isPrimitive() )
		processPrimitive(object, field, value);
	else if( field.type.isEnum() )
		processEnum(object, field, value);
	else
		assert(0);
}

//-----------------------------------//

void JsonDeserializer::processEnum(Object* object, Field& field, const Json::Value& value)
{
	std::string name = value.asString();
	const Enum& type = (Enum&) field.type;
	uint enumValue = type.getValue(name);
	field.set<uint>(object, enumValue);
}

//-----------------------------------//

void JsonDeserializer::processArray(Object* object, Field& field, const Json::Value& value)
{
	void* address =  (byte*) object + field.offset;
	byte* begin = processArrayPointer((Object*)address, field, value.size());

	int size = field.size;
	if( field.isPointer() )	size = field.pointerSize;

	for(uint i = 0; i < value.size(); i++ )
	{
		const Json::Value& arrayValue = value[i];
		
		void* element = begin + i * size;
		processArrayElement(element, field, arrayValue);
	}
}

//-----------------------------------//

void JsonDeserializer::processArrayElement(void* element, Field& field, const Json::Value& value)
{
	if( field.type.isPrimitive() )
	{
		processPrimitive(element, field, value);
	}
	else if( field.type.isClass() )
	{
		Object* object = processObject(value);

		if( field.qualifiers & Qualifier::SharedPointer )
		{
			((std::shared_ptr<Object>*) element)->reset(object);
		}
		else if( field.qualifiers & Qualifier::RefPointer )
		{
			ReferenceCounted* ref = (ReferenceCounted*) object;
			((RefPtr<ReferenceCounted>*) element)->reset(ref);
		}
		else
		{
			int size = field.size;
			if( field.isPointer() )	size = field.pointerSize;
			
			#pragma TODO("Placement new the objects in their memory")
			memcpy(element, object, size);
			// Potential memory leak here.
		}
	}
}

//-----------------------------------//

byte* JsonDeserializer::processArrayPointer(Object* address, Field& field, int size)
{
	if( field.qualifiers & Qualifier::SharedPointer )
	{
		typedef std::vector<std::shared_ptr<Object>> ObjectSharedPtrArray;
		ObjectSharedPtrArray* array = (ObjectSharedPtrArray*) address;
		array->resize(size);

		return (byte*) &array->front();
	}
	else if( field.qualifiers & Qualifier::RefPointer )
	{
		typedef std::vector<RefPtr<ReferenceCounted>> ObjectRefPtrArray;
		ObjectRefPtrArray* array = (ObjectRefPtrArray*) address;
		array->resize(size);
		
		return (byte*) &array->front();
	}
	else
	{
		typedef std::vector<Object*> ObjectRawPtrArray;
		ObjectRawPtrArray* array = (ObjectRawPtrArray*) address;
		array->resize(size);
		
		return (byte*) &array->front();
	}
}

//-----------------------------------//

#define setValue(T, val) *(T*)address = val;

void JsonDeserializer::processPrimitive(void* address, Field& field, const Json::Value& value)
{
	const Primitive& type = (const Primitive&) field.type;
	
	if( !field.isArray() )
		address = (byte*) address + field.offset;
	
	if( type.isBool() )
	{
		bool val = value.asBool();
		setValue(bool, val);
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int val = value.asInt();
		setValue(int, val);
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float val = (float) value.asDouble();
		setValue(float, val);
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string val = value.asString();
		setValue(std::string, val);
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color val = convertValueToColor(value);
		setValue(Color, val);
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3 val = convertValueToVector3(value);
		setValue(Vector3, val);
	}
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion val = convertValueToQuaternion(value);
		setValue(Quaternion, val);
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		uint val = value.asUInt();
		setValue(uint, val);
	}
}

//-----------------------------------//

} // end namespace

#endif