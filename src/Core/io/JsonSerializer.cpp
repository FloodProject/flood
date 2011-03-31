/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef VAPOR_SERIALIZATION_JSON

#include "io/JsonSerializer.h"
#include "Core/Memory.h"
#include "Core/Utilities.h"
#include "Core/ReferenceCount.h"
#include "Core/Log.h"
#include <jansson.h>

namespace vapor {

//-----------------------------------//

static Vector3 convertValueToVector3( json_t* value )
{
	Vector3 v;
	json_unpack(value, "[g,g,g]", &v.x, &v.y, &v.z);
	return v;
}

//-----------------------------------//

static Color convertValueToColor( json_t* value )
{
	Color c;
	json_unpack(value, "[g,g,g,g]", &c.r, &c.g, &c.b, &c.a);
	return c;
}

//-----------------------------------//

static Quaternion convertValueToQuaternion( json_t* value )
{
	Quaternion q;
	json_unpack(value, "[g,g,g,g]", &q.x, &q.y, &q.z, &q.w);
	return q;
}

//-----------------------------------//

static EulerAngles convertValueToEulerAngles( json_t* value )
{

	EulerAngles a;
	json_unpack(value, "[g,g,g]", &a.x, &a.y, &a.z);
	return a;
}

//-----------------------------------//

static json_t* convertVector3( const Vector3& vec )
{
	return json_pack("[g,g,g]", vec.x, vec.y, vec.z);
}

//-----------------------------------//

static json_t* convertColor( const Color& c )
{
	return json_pack("[g,g,g,g]", c.r, c.g, c.b, c.a);
}

//-----------------------------------//

static json_t* convertQuaternion( const Quaternion& q )
{
	return json_pack("[g,g,g,g]", q.x, q.y, q.z, q.w);
}

//-----------------------------------//


JsonSerializer::JsonSerializer(Stream& stream)
	: stream(stream)
{
	values.push(rootValue);
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

	String text = json_dumps(rootValue, 0);
	StreamWriteString(&stream, text);
	StreamClose(&stream);
}

//-----------------------------------//

void JsonSerializer::processClassBegin(const Class& type, bool parent)
{
	if(parent) return;
	json_t* klass = json_object();
	json_object_set_new(values.top(), type.name.c_str(), klass);
	values.push( klass );
}

//-----------------------------------//

void JsonSerializer::processClassEnd(const Class& type, bool parent)
{
	if(parent) return;
	values.pop();
}

//-----------------------------------//

void JsonSerializer::processFieldBegin( const Field& field )
{
#if 0
	json_t* klass = json_object();
	json_object_set_new(values.top(), type.name.c_str(), klass);

	json_t& top = *values.top();
	values.push( &top[field.name] );
#endif
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
#if 0
	json_t& top = *values.top();
	values.push( &top[(uint)i] );
#endif
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
	
void JsonSerializer::processEnumElement(int value, const String& name)
{
#if 0
	json_t& top = *values.top();
	top = name;
#endif
}

//-----------------------------------//

void JsonSerializer::processBool( const Primitive& prim, bool value )
{
#if 0
	json_t& top = *values.top();
	top = value;
#endif
}

//-----------------------------------//

void JsonSerializer::processInt( const Primitive& prim, int32 value )
{
#if 0
	json_t& top = *values.top();
	top = (json_t::Int) value;
#endif
}

//-----------------------------------//

void JsonSerializer::processUInt( const Primitive& prim, uint32 value )
{
#if 0
	json_t& top = *values.top();
	top = (json_t::UInt) value;
#endif
}

//-----------------------------------//

void JsonSerializer::processFloat( const Primitive& prim, float value )
{
#if 0
	json_t& top = *values.top();
	top = value;
#endif
}

//-----------------------------------//

void JsonSerializer::processString( const Primitive& prim, const String& value )
{
#if 0
	json_t& top = *values.top();
	top = value;
#endif
}

//-----------------------------------//

void JsonSerializer::processVector3( const Primitive& prim, const Vector3& value )
{
#if 0
	json_t& top = *values.top();
	top = convertVector3(value);
#endif
}

//-----------------------------------//

void JsonSerializer::processQuaternion( const Primitive& prim, const Quaternion& value )
{
#if 0
	json_t& top = *values.top();
	top = convertQuaternion(value);
#endif
}

//-----------------------------------//

void JsonSerializer::processColor( const Primitive& prim, const Color& value )
{
#if 0
	json_t& top = *values.top();
	top = convertColor(value);
#endif
}

//-----------------------------------//

void JsonSerializer::processBitfield( const Primitive& prim, const uint32& value )
{
#if 0
	json_t& top = *values.top();
	top = (json_t::UInt) value;
#endif
}

//----------------------------------------------------------------------//

JsonDeserializer::JsonDeserializer(Stream& stream)
	: stream(stream)
	, registry(Type::GetRegistry())
{ }

//-----------------------------------//

static void* JsonAllocate(size_t size)
{
	MemoryAllocator* mem = AllocatorGetDefault();
	return mem->allocate(mem, 0);
}

static void JsonDeallocate(void* p)
{
	MemoryAllocator* mem = AllocatorGetDefault();
	return mem->deallocate(mem, p, 0);
}

Object* JsonDeserializer::deserialize()
{
	String text;
	StreamReadString(&stream, text);
	StreamClose(&stream);

	json_set_alloc_funcs(JsonAllocate, JsonDeallocate);

	json_t* rootValue;

	{
		LocaleSwitch locale;
		rootValue = json_loads(text.c_str(), 0, nullptr);
	}

	if( !rootValue ) return nullptr;
	return processObject(rootValue);
}

//-----------------------------------//

Object* JsonDeserializer::processObject(json_t* value)
{
	Object* object = nullptr;

	if( !json_is_object(value) )
		return nullptr;

	void* iter = json_object_iter(value);
	
	while(iter)
	{
		const char* key = json_object_iter_key(iter);
		json_t* field = json_object_iter_value(iter);

		const String& name = key;

		const Class* klass = (Class*) registry.getType(name);
		if( !klass ) continue;

		object = (Object*) klass->createInstance();
		
		if( !object ) continue;

		processFields(object, field);
		object->fixUp();

		iter = json_object_iter_next(value, iter);
	}

	return object;
}

//-----------------------------------//

void JsonDeserializer::processFields(Object* object, json_t* value)
{
	if( !object ) return;

	const Class& type = (Class&) object->getType();


	void* iter = json_object_iter(value);
	
	while(iter)
	{
		const char* key = json_object_iter_key(iter);
		json_t* fieldval = json_object_iter_value(iter);

		const String& name = key;

		Field* field = type.getField(name);
		
		if( !field ) continue;
		if( json_is_null(fieldval) ) continue;

		processField(object, *field, fieldval);
		
		iter = json_object_iter_next(value, iter);
	}
}

//-----------------------------------//

void JsonDeserializer::processField(Object* object, Field& field, json_t* value)
{
	if( field.isArray() && json_is_array(value) )
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

void JsonDeserializer::processEnum(Object* object, Field& field, json_t* value)
{
	String name = json_string_value(value);
	const Enum& type = (Enum&) field.type;
	uint enumValue = type.getValue(name);
	field.set<uint>(object, enumValue);
}

//-----------------------------------//

void JsonDeserializer::processArray(Object* object, Field& field, json_t* value)
{
	assert( json_is_array(value) );
	size_t arrsize = json_array_size(value);

	void* address =  (byte*) object + field.offset;
	byte* begin = processArrayPointer((Object*)address, field, arrsize);

	int size = field.size;
	if( field.isPointer() )	size = field.pointerSize;

	for(size_t i = 0; i < arrsize; i++ )
	{
		json_t* arrayValue = json_array_get(value, i);
		
		void* element = begin + i * size;
		processArrayElement(element, field, arrayValue);
	}
}

//-----------------------------------//

void JsonDeserializer::processArrayElement(void* element, Field& field, json_t* value)
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

void JsonDeserializer::processPrimitive(void* address, Field& field, json_t* value)
{
	const Primitive& type = (const Primitive&) field.type;
	
	if( !field.isArray() )
		address = (byte*) address + field.offset;
	
	switch(type.type)
	{
	case PrimitiveType::Bool:
	{
		assert( json_is_boolean(value) );
		bool val = json_integer_value(value) != 0;
		setValue(bool, val);
	}
	//-----------------------------------//
	case PrimitiveType::Integer:
	{
		int32 val = (int32) json_integer_value(value);
		setValue(int32, val);
	}
	//-----------------------------------//
	case PrimitiveType::Float:
	{
		float val = (float) json_real_value(value);
		setValue(float, val);
	}
	//-----------------------------------//
	case PrimitiveType::String:
	{
		String val = json_string_value(value);
		setValue(String, val);
	}
	//-----------------------------------//
	case PrimitiveType::Color:
	{
		Color val = convertValueToColor(value);
		setValue(Color, val);
	}
	//-----------------------------------//
	case PrimitiveType::Vector3:
	{
		Vector3 val = convertValueToVector3(value);
		setValue(Vector3, val);
	}
	//-----------------------------------//
	case PrimitiveType::Quaternion:
	{
		Quaternion val = convertValueToQuaternion(value);
		setValue(Quaternion, val);
	}
	//-----------------------------------//
	case PrimitiveType::Bitfield:
	{
		uint32 val = json_integer_value(value);
		setValue(uint32, val);
	}
	}
}

//-----------------------------------//

} // end namespace

#endif