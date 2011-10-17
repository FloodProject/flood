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
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Utilities.h"
#include "Core/References.h"
#include "Core/Stream.h"
#include "Core/Log.h"
#include "Core/SerializationHelpers.h"

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

	Quaternion null;
	return null;
}

//-----------------------------------//

struct API_CORE SerializerJSON : public Serializer
{
	// Root JSON value.
	json_t* rootValue;
	
	// Stack of JSON values.
	std::vector<json_t*> values;
	std::vector<json_t*> arrays;
};

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
		json_object_set_new(klass, ctx->klass->name, json->values.back());
		
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
	int32 value = *vc.i32;
	
	const char* name = EnumGetValueName(ctx->enume, value);
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
		bool& b = *vc.b;
		value = b ? json_true() : json_false();
		break;
	}
	case Primitive::Int32:
	{
		int32& i = *vc.i32;
		value = json_integer(i);
		break;
	}
	case Primitive::Uint32:
	{
		uint32& i = *vc.u32;
		value = json_integer(i);
		break;
	}
	case Primitive::Float:
	{
		float& f = *vc.f32;
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
		Color& c = *vc.c;
		value = json_pack("[f,f,f,f]", c.r, c.g, c.b, c.a);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3& v = *vc.v;
		value = json_pack("[f,f,f]", v.x, v.y, v.z);
		break;
	}
	case Primitive::Quaternion:
	{
		Quaternion& q = *vc.q;
		value = json_pack("[f,f,f,f]", q.x, q.y, q.z, q.w);
		break;
	}
	default:
		assert( false );
	}

	json->values.push_back(value);
}

//-----------------------------------//

static void DeserializeEnum( ReflectionContext* context, json_t* value )
{
	const char* name = json_string_value(value);
	int32 enumValue = EnumGetValue(context->enume, name);
	FieldSet<int32>(context->field, context->object, enumValue);
}

//-----------------------------------//

#define SetFieldValue(T, val) FieldSet<T>(context->field, context->object, val);

static void DeserializePrimitive( ReflectionContext* context, json_t* value )
{
	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		assert( json_is_boolean(value) );
		bool val = json_typeof(value) == JSON_TRUE;
		SetFieldValue(bool, val);
		break;
	}
	case Primitive::Int32:
	{
		int32 val = (int32) json_integer_value(value);
		SetFieldValue(int32, val);
		break;
	}
	case Primitive::Uint32:
	{
		uint32 val = (uint32) json_integer_value(value);
		SetFieldValue(uint32, val);
		break;
	}
	case Primitive::Float:
	{
		float val = (float) json_real_value(value);
		SetFieldValue(float, val);
		break;
	}
	case Primitive::String:
	{
		String val = json_string_value(value);
		SetFieldValue(String, val);
		break;
	}
	case Primitive::Color:
	{
		Color val = ConvertValueToColor(value);
		SetFieldValue(Color, val);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3 val = ConvertValueToVector3(value);
		SetFieldValue(Vector3, val);
		break;
	}
	case Primitive::Quaternion:
	{
		Quaternion val = ConvertValueToQuaternion(value);
		SetFieldValue(Quaternion, val);
		break;
	}
	default:
		assert(0 && "Unknown primitive type");
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, json_t* value );

static void DeserializeArrayElement( ReflectionContext* context, json_t* value, void* address )
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
		Object* object = DeserializeComposite(context, value);
		PointerSetObject(field, address, object);
		break;
	} }
}

//-----------------------------------//

static void DeserializeArray( ReflectionContext* context, json_t* value )
{
	// Some old serialized files don't properly encode the components of an entity
	// as an array, so don't force the JSON value to be an array.
	
	/*assert( json_is_array(value) );*/

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
		
			// Calculate the address of the next array element.
			void* element = (byte*) begin + elementSize * i;
			DeserializeArrayElement(context, arrayValue, element);
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

			// Calculate the address of the next array element.
			void* element = (byte*) begin + elementSize * i++;
			DeserializeArrayElement(context, arrayValue, element);

			json_decref(arrayValue);
		}
	}
}

//-----------------------------------//

extern ReferenceCounted* NullResolve(HandleId);
extern void NullDestroy(HandleId);

static void DeserializeField( ReflectionContext* context, json_t* value )
{
	const Field* field = context->field;

	if( FieldIsArray(field) )
	{
		DeserializeArray(context, value);
		return;
	}
	
	switch(field->type->type)
	{
	case Type::Composite:
	{
		Class* composite = context->composite;
		context->composite = (Class*) field->type;
		
		if( FieldIsHandle(field) )
		{
			if( !json_is_object(value) )
			{
				LogDebug("Can't deserialize handle '%s'", field->name);
				return;
			}

			void* iter = json_object_iter(value);
			void* iter2 = json_object_iter( json_object_iter_value(iter) );
			json_t* val = json_object_iter_value(iter2);
			const char* name = json_string_value(val);

			ReflectionHandleContext handleContext;
			ReflectionFindHandleContext((Class*) field->type, handleContext);
			
			HandleId id = handleContext.deserialize(name);
			if(id == HandleInvalid) return;

			void* address = ClassGetFieldAddress(context->object, field);
			
			typedef Handle<Object, NullResolve, NullDestroy> ObjectHandle;
			ObjectHandle* handleObject = (ObjectHandle*) address;
			handleObject->setId(id);
			
			ReferenceAdd( HandleFind(handleContext.handles, id) );
		}
		else if( FieldIsPointer(field) )
		{
			Object* object = DeserializeComposite(context, value);
			void* address = ClassGetFieldAddress(context->object, field);

			PointerSetObject(field, address, object);
		}

		context->composite = composite;
		break;
	}
	case Type::Primitive:
	{
		context->primitive = (Primitive*) context->field->type;
		DeserializePrimitive(context, value);
		break;
	}
	case Type::Enumeration:
	{
		context->enume = (Enum*) context->field->type;
		DeserializeEnum(context, value);
		break;
	} }
}

//-----------------------------------//

static void DeserializeFields( ReflectionContext* context, json_t* value )
{
	void* iter = json_object_iter(value);
	for(; iter; iter = json_object_iter_next(value, iter))
	{
		const char* key = json_object_iter_key(iter);
		json_t* val = json_object_iter_value(iter);

		Class* composite = context->composite;
		const Field* field = context->field;

		Field* newField = ClassGetField(composite, key);
		
		if( !newField )
		{
			LogDebug("Unknown field '%s' of class '%s'", key, composite->name);
			continue;
		}
		
		if( json_is_null(val) ) continue;

		context->field = newField;
	
		DeserializeField(context, val);

		context->field = field;
		context->composite = composite;
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, json_t* value )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;

	if( !json_is_object(value) ) return 0;

	void* iter = json_object_iter(value);
	const char* key = json_object_iter_key(iter);
	json_t* iterValue = json_object_iter_value(iter);

	Class* newClass = (Class*) context->composite;

	#pragma TODO("Remove this compatibility check")

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

	Object* newObject = (Object*) ClassCreateInstance(newClass, json->alloc);
	if( !newObject ) return 0;

	Class* klass = context->klass;
	Class* composite = context->composite;
	Object* object = context->object;

	context->klass = newClass;
	context->composite = newClass;
	context->object = newObject;

	DeserializeFields(context, value);

	if( ClassInherits(newClass, ReflectionGetType(Object)) )
		newObject->fixUp();

	context->object = object;
	context->composite = composite;
	context->klass = klass;

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
	Object* object = DeserializeComposite(context, rootValue);
	json_decref(rootValue);

	return object;
}

//-----------------------------------//

static bool SerializeSave( Serializer* serializer, Object* object )
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

Serializer* SerializerCreateJSON(Allocator* alloc)
{
	//json_set_alloc_funcs(JsonAllocate, JsonDeallocate);

	SerializerJSON* serializer = Allocate(SerializerJSON, alloc);
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

	return serializer;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif