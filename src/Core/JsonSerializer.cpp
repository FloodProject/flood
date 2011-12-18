/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION_JSON

#include "Core/Serialization.h"
#include "Core/SerializationHelpers.h"

#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Utilities.h"
#include "Core/References.h"
#include "Core/Stream.h"
#include "Core/Log.h"

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/EulerAngles.h"
#include "Math/Color.h"

#include <jansson.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Vector3 ConvertValueToVector3( json_t* value )
{
	double x, y, z;
	json_unpack(value, "[f,f,f]", &x, &y, &z);

	Vector3 v;
	v.x = float(x);
	v.y = float(y);
	v.z = float(z);

	return v;
}

//-----------------------------------//

static Color ConvertValueToColor( json_t* value )
{
	double r, g, b, a;
	json_unpack(value, "[f,f,f,f]", &r, &g, &b, &a);

	Color c;
	c.r = float(r);
	c.g = float(g);
	c.b = float(b);
	c.a = float(a);

	return c;
}

//-----------------------------------//

static Quaternion ConvertValueToQuaternion( json_t* value )
{
	// For compatibility with old serialized files, check if the rotation
	// was encoded as euler angles, in which case it will have 3 elements.

	size_t length = json_array_size(value);

	if(length == 4)
	{
		double x, y, z, w;
		json_unpack(value, "[f,f,f,f]", &x, &y, &z, &w);

		Quaternion q;
		q.x = float(x);
		q.y = float(y);
		q.z = float(z);
		q.w = float(w);

		return q;
	}
	else if(length == 3)
	{
		EulerAngles ang;

		double x, y, z;
		json_unpack(value, "[f,f,f]", &x, &y, &z);

		ang.x = float(x);
		ang.y = float(y);
		ang.z = float(z);
		
		return Quaternion(ang);
	}

	LogDebug("Invalid JSON value size for quaternion");

	static Quaternion null;
	return null;
}

//-----------------------------------//

ValueContext ConvertValueToPrimitive( Primitive::PrimitiveType type, json_t* value )
{
	ValueContext vc;

	switch(type)
	{
	case Primitive::Bool:
	{
		assert( json_is_boolean(value) );
		vc.b = json_typeof(value) == JSON_TRUE;
		break;
	}
	case Primitive::Int16:
	{
		vc.i16 = (int16) json_integer_value(value);
		break;
	}
	case Primitive::Uint16:
	{
		vc.u16 = (uint16) json_integer_value(value);
		break;
	}
	case Primitive::Int32:
	{
		vc.i32 = (int32) json_integer_value(value);
		break;
	}
	case Primitive::Uint32:
	{
		vc.u32 = (uint32) json_integer_value(value);
		break;
	}
	case Primitive::Int64:
	{
		vc.i64 = (int64) json_integer_value(value);
		break;
	}
	case Primitive::Uint64:
	{
		vc.u64 = (uint64) json_integer_value(value);
		break;
	}
	case Primitive::Float:
	{
		vc.f32 = (float) json_real_value(value);
		break;
	}
	case Primitive::String:
	{
		vc.cs = json_string_value(value);
		break;
	}
	case Primitive::Color:
	{
		vc.c = ConvertValueToColor(value);
		break;
	}
	case Primitive::Vector3:
	{
		vc.v = ConvertValueToVector3(value);
		break;
	}
	case Primitive::Quaternion:
	{
		vc.q = ConvertValueToQuaternion(value);
		break;
	}
	default:
		assert(0 && "Unknown primitive type");
	}

	return vc;
}

//-----------------------------------//

static void SerializeArray(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;

	if(wt == ReflectionWalkType::Begin)
	{
		json_t* array = json_array();	
		json->values.push_back( array );
	}
	else if(wt == ReflectionWalkType::ElementEnd)
	{
		json_t* element = json->values.back();
		json->values.pop_back();
		json_array_append_new(json->values.back(), element);
	}
}

//-----------------------------------//

// A composite object is serialized via two JSON objects. The first object has just
// one key and contains the name of the concrete class of the type. This is used
// to handle polymorphism, where a derived instance is used where a base is expected.

static void SerializeComposite(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;
	
	if(wt == ReflectionWalkType::Begin)
	{
		json_t* klass = json_object();
		json->values.push_back( klass );
	}
	else if(wt == ReflectionWalkType::End)
	{
		json_t* klass = json_object();
		json_object_set_new(klass, ctx->objectClass->name, json->values.back());
		
		json->values.pop_back();
		json->values.push_back(klass);
	}
}

//-----------------------------------//

static void SerializeField(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;

	if(wt == ReflectionWalkType::End)
	{
		json_t* field = json->values.back();
		json->values.pop_back();
		json_object_set_new(json->values.back(), ctx->field->name, field);
		
		return;
	}
}

//-----------------------------------//
	
static void SerializeEnum(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;
	
	ValueContext& vc = ctx->valueContext;
	const char* name = EnumGetValueName(ctx->enume, vc.i32);
	assert( name != nullptr );

	json_t* str = json_string(name);
	json->values.push_back(str);
}

//-----------------------------------//

static void SerializePrimitive( ReflectionContext* context, ReflectionWalkType::Enum wt )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;
	ValueContext& vc = context->valueContext;

	json_t* value = nullptr;

	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		bool& b = vc.b;
		value = b ? json_true() : json_false();
		break;
	}
	case Primitive::Int32:
	{
		int32& i = vc.i32;
		value = json_integer(i);
		break;
	}
	case Primitive::Uint32:
	{
		uint32& i = vc.u32;
		value = json_integer(i);
		break;
	}
	case Primitive::Float:
	{
		float& f = vc.f32;
		value = json_real(f);
		break;
	}
	case Primitive::String:
	{
		String& s = *vc.s;
		value = json_string( s.c_str() );
		break;
	}
	case Primitive::Color:
	{
		ColorP& c = vc.c;
		value = json_pack("[f,f,f,f]", c.r, c.g, c.b, c.a);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3P& v = vc.v;
		value = json_pack("[f,f,f]", v.x, v.y, v.z);
		break;
	}
	case Primitive::Quaternion:
	{
		QuaternionP& q = vc.q;
		value = json_pack("[f,f,f,f]", q.x, q.y, q.z, q.w);
		break;
	}
	default:
		assert( false );
	}

	json->values.push_back(value);
}

//-----------------------------------//

static void DeserializeEnum( ReflectionContext* context )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* value = json->values.back();

	const char* name = json_string_value(value);
	int32 enumValue = EnumGetValue(context->enume, name);
	FieldSet<int32>(context->field, context->object, enumValue);
}

//-----------------------------------//

#define SetFieldValue(T, val) FieldSet<T>(context->field, context->object, val);

static void DeserializePrimitive( ReflectionContext* context )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* value = json->values.back();

	ValueContext vc = ConvertValueToPrimitive(context->primitive->type, value);

	switch(context->primitive->type)
	{
	case Primitive::Bool: { SetFieldValue(bool, vc.b); break; }
	case Primitive::Int32: { SetFieldValue(int32, vc.i32); break; }
	case Primitive::Uint32: { SetFieldValue(uint32, vc.u32); break; }
	case Primitive::Float: { SetFieldValue(float, vc.f32); break; }
	case Primitive::String: { SetFieldValue(String, vc.cs); break; }
	case Primitive::Color: { SetFieldValue(ColorP, vc.c); break; }
	case Primitive::Vector3: { SetFieldValue(Vector3P, vc.v); break; }
	case Primitive::Quaternion: { SetFieldValue(QuaternionP, vc.q); break; }
	default: assert(0 && "Unknown primitive type");
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, Object* newObject );

static void DeserializeArrayElement( ReflectionContext* context, void* address )
{
	const Field* field = context->field;

	switch(field->type->type)
	{
	case Type::Primitive:
	{
		assert(0 && "Not implemented");
		break;
	}
	case Type::Enumeration:
	{
		assert(0 && "Not implemented");
		break;
	}
	case Type::Composite:
	{
		context->composite = (Class*) field->type;

		if( !FieldIsPointer(field) )
		{
			Object* object = DeserializeComposite(context, (Object*) address);
		}
		else
		{
			Object* object = DeserializeComposite(context, 0);
			PointerSetObject(field, address, object);
		}
		
		break;
	} }

	SerializerJSON* json = (SerializerJSON*) context->userData;
	json->values.pop_back();
}

//-----------------------------------//

static void DeserializeArray( ReflectionContext* context )
{
	// Some old serialized files don't properly encode the components of an entity
	// as an array, so don't force the JSON value to be an array.

	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* value = json->values.back();

	const Field* field = context->field;
	uint16 elementSize = ReflectionArrayGetElementSize(field);

	size_t size = 0;

	bool isArray = json_is_array(value);

	if( isArray )
		size = json_array_size(value);
	else
		size = json_object_size(value);

	if( size == 0 ) return;

	void* address = ClassGetFieldAddress(context->object, field);
	void* begin = ReflectionArrayResize(context, address, size);

	if( isArray )
	{
		for( size_t i = 0; i < size; i++ )
		{
			json_t* arrayValue = json_array_get(value, i);
			json->values.push_back(arrayValue);

			// Calculate the address of the next array element.
			void* element = (byte*) begin + elementSize * i;
			DeserializeArrayElement(context, element);
		}
	}
	else
	{
		size_t i = 0;
		void* iter = json_object_iter(value);

		for(; iter; iter = json_object_iter_next(value, iter))
		{
			const char* key = json_object_iter_key(iter);
			json_t* val = json_object_iter_value(iter);

			json_t* arrayValue = json_object();
			json_object_set(arrayValue, key, val);

			json->values.push_back(arrayValue);

			// Calculate the address of the next array element.
			void* element = (byte*) begin + elementSize * i++;
			DeserializeArrayElement(context, element);

			json_decref(arrayValue);
		}
	}
}

//-----------------------------------//

extern ReferenceCounted* NullResolve(HandleId);
extern void NullDestroy(HandleId);

static bool DeserializeHandleName( ReflectionContext* context, const char* name )
{
	const Field* field = context->field;

	ReflectionHandleContext handleContext;
	ReflectionFindHandleContext((Class*) field->type, handleContext);
			
	HandleId id = handleContext.deserialize(name);
	if(id == HandleInvalid) return false;

	typedef Handle<Object, NullResolve, NullDestroy> ObjectHandle;
	ObjectHandle handleObject;
	handleObject.setId(id);
				
	FieldSet(field, context->object, handleObject);

	return true;
}

//-----------------------------------//

static void DeserializeHandle( ReflectionContext* context, json_t* value )
{
	const Field* field = context->field;

	if( json_is_string(value) )
	{
		const char* name = json_string_value(value);
		DeserializeHandleName(context, name);
	}

	if( !json_is_object(value) )
	{
		LogDebug("Can't deserialize handle '%s'", field->name);
		return;
	}

	void* iter = json_object_iter(value);

	json_t* obj = json_object_iter_value(iter);
	void* iter2 = json_object_iter( obj );

	do
	{
		json_t* val = json_object_iter_value(iter2);
		if( !json_is_string(val) ) continue;

		const char* name = json_string_value(val);
			
		if( DeserializeHandleName(context, name) )
			break;
	}
	while( iter2 = json_object_iter_next(obj, iter2) );
}

//-----------------------------------//

static void DeserializeField( ReflectionContext* context, ReflectionWalkType::Enum wt )
{
	const Field* field = context->field;

	if( field->serialize )
	{
		field->serialize(context, wt);
		return;
	}
	else if( FieldIsArray(field) )
	{
		DeserializeArray(context);
		return;
	}

	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* value = json->values.back();

	switch(field->type->type)
	{
	case Type::Composite:
	{
		Class* composite = context->composite;
		context->composite = (Class*) field->type;
		
		if( FieldIsHandle(field) )
		{
			DeserializeHandle(context, value);
		}
		else if( FieldIsPointer(field) )
		{
			Object* object = DeserializeComposite(context, 0);
			
			void* address = ClassGetFieldAddress(context->object, field);
			PointerSetObject(field, address, object);
		}
		else
		{
			void* address = ClassGetFieldAddress(context->object, field);
			Object* object = DeserializeComposite(context, (Object*) address);
		}

		context->composite = composite;
		break;
	}
	case Type::Primitive:
	{
		context->primitive = (Primitive*) context->field->type;
		DeserializePrimitive(context);
		break;
	}
	case Type::Enumeration:
	{
		context->enume = (Enum*) context->field->type;
		DeserializeEnum(context);
		break;
	} }
}

//-----------------------------------//

static void DeserializeFields( ReflectionContext* context, ReflectionWalkType::Enum )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* value = json->values.back();

	void* iter = json_object_iter(value);
	for(; iter; iter = json_object_iter_next(value, iter))
	{
		const char* key = json_object_iter_key(iter);
		json_t* fieldValue = json_object_iter_value(iter);

		Class* composite = context->composite;
		const Field* field = context->field;

		Field* newField = ClassGetField(composite, key);
		
		if( !newField )
		{
			LogDebug("Unknown field '%s' of class '%s'", key, composite->name);
			continue;
		}
		
		if( json_is_null(fieldValue) ) continue;

		context->field = newField;

		json->values.push_back(fieldValue);
		DeserializeField(context, ReflectionWalkType::Element);
		json->values.pop_back();

		context->field = field;
		context->composite = composite;
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, Object* newObject )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;
	json_t* value = json->values.back();

	if( !json_is_object(value) ) return 0;

	void* iter = json_object_iter(value);
	const char* key = json_object_iter_key(iter);
	json_t* iterValue = json_object_iter_value(iter);

	Class* newClass = (Class*) context->composite;

	#pragma TODO("Remove JSON class compatibility check")

	// Use explicit class if object has one to handle polymorphism.
	Class* explicitClass = (Class*) ReflectionFindType(key);
	
	// If there is no field class, require an explicit class.
	if( !newClass && !explicitClass )
		return 0;
	
	if( !newClass ) newClass = explicitClass;

	// Test if the class is compatible with the field we are dealing with.
	if( explicitClass && ClassInherits(explicitClass, newClass) )
	{
		newClass = explicitClass;
		value = iterValue;
	}

	if( !newClass || ClassIsAbstract(newClass) )
		return 0;

	if( !newObject )
		newObject = (Object*) ClassCreateInstance(newClass, json->alloc);

	if( !newObject ) return 0;

	Class* objectClass = context->objectClass;
	Class* composite = context->composite;
	Object* object = context->object;

	context->objectClass = newClass;
	context->composite = newClass;
	context->object = newObject;

	json->values.push_back(value);

	if( newClass->serialize )
		newClass->serialize(context, ReflectionWalkType::Begin);
	else
		DeserializeFields(context, ReflectionWalkType::Begin);

	json->values.pop_back();

	if( ClassInherits(newClass, ReflectionGetType(Object)) )
		newObject->fixUp();

	context->object = object;
	context->composite = composite;
	context->objectClass = objectClass;

	return newObject;
}

//-----------------------------------//

static Object* SerializeLoad( Serializer* serializer )
{
	SerializerJSON* json = (SerializerJSON*) serializer;
	
	json->rootValue = nullptr;
	json->values.clear();
	json->arrays.clear();

	String text;
	StreamReadString(json->stream, text);
	StreamClose(json->stream);

	json_t* rootValue;

	LocaleSwitch locale;
	rootValue = json_loads(text.c_str(), 0, nullptr);
	
	if( !rootValue )
	{
		LogError("Could not parse JSON text of '%s'", json->stream->path.c_str());
		return nullptr;
	}

	json->rootValue = rootValue;
	
	ReflectionContext* context = &serializer->deserializeContext;
	Object* object = serializer->object;

	json->values.push_back(rootValue);

	object = DeserializeComposite(context, object);

	json->values.pop_back();
	assert( json->values.empty() );

	json_decref(rootValue);

	return object;
}

//-----------------------------------//

static bool SerializeSave( Serializer* serializer, const Object* object )
{
	SerializerJSON* json = (SerializerJSON*) serializer;

	json->rootValue = nullptr;
	json->values.clear();
	json->arrays.clear();

	ReflectionWalk(object, &json->serializeContext);
	assert( json->values.size() == 1 );
		
	json_t* rootValue = json->values.back();

	// Always switch to the platform independent "C" locale when writing
	// JSON, else the library will format the data erroneously.

	LocaleSwitch locale;

	size_t flags = JSON_INDENT(4) | JSON_PRESERVE_ORDER;
	
	char* dump = json_dumps(rootValue, flags);
	
	if( dump )
		StreamWriteString(json->stream, dump);
	
	StreamClose(json->stream);

	Deallocate(dump);
	json_decref(rootValue);

	return true;
}

//-----------------------------------//

#if 0
static void* JsonAllocate(size_t size)
{
	Allocator* alloc = AllocatorGetHeap();
	return alloc->allocate(alloc, 0, 0);
}

static void JsonDeallocate(void* p)
{
	Allocator* alloc = AllocatorGetHeap();
	return alloc->deallocate(alloc, p);
}
#endif

Serializer* SerializerCreateJSON(Allocator* alloc)
{
	#pragma TODO("Hook memory allocators to JSON library")
	//json_set_alloc_funcs(JsonAllocate, JsonDeallocate);

	SerializerJSON* serializer = Allocate(alloc, SerializerJSON);
	serializer->load = SerializeLoad;
	serializer->save = SerializeSave;
	serializer->alloc = alloc;

	ReflectionContext& sCtx = serializer->serializeContext;
	sCtx.userData = serializer;
	sCtx.walkArray = SerializeArray;
	sCtx.walkComposite = SerializeComposite;
	sCtx.walkCompositeField = SerializeField;
	sCtx.walkPrimitive = SerializePrimitive;
	sCtx.walkEnum = SerializeEnum;

	ReflectionContext& dCtx = serializer->deserializeContext;
	dCtx.userData = serializer;
	dCtx.walkCompositeFields = DeserializeFields;
	dCtx.walkCompositeField = DeserializeField;

	return serializer;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif