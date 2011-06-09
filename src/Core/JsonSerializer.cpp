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
#include "Core/ReferenceCount.h"
#include "Core/Stream.h"
#include "Core/Log.h"

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/EulerAngles.h"
#include "Math/Color.h"

#include <stack>
#include <jansson.h>

NAMESPACE_BEGIN

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
	double x, y, z, w;
	json_unpack(value, "[f,f,f,f]", &x, &y, &z, &w);

	Quaternion q;
	q.x = float(x);
	q.y = float(y);
	q.z = float(z);
	q.w = float(w);

	return q;
}

//-----------------------------------//
#if 0
static EulerAngles ConvertValueToEulerAngles( json_t* value )
{

	EulerAngles a;
	json_unpack(value, "[g,g,g]", &a.x, &a.y, &a.z);
	return a;
}
#endif
//-----------------------------------//

static json_t* ConvertVector3( const Vector3& vec )
{
	return json_pack("[f,f,f]", vec.x, vec.y, vec.z);
}

//-----------------------------------//

static json_t* ConvertColor( const Color& c )
{
	return json_pack("[f,f,f,f]", c.r, c.g, c.b, c.a);
}

//-----------------------------------//

static json_t* ConvertQuaternion( const Quaternion& q )
{
	return json_pack("[f,f,f,f]", q.x, q.y, q.z, q.w);
}

//-----------------------------------//

struct API_CORE SerializerJSON : public Serializer
{
	// Root JSON value.
	json_t* rootValue;
	
	// Stack of JSON values.
	std::stack<json_t*> values;
	std::stack<json_t*> arrays;
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
		json->values.push( array );
	}
	else if(wt == ReflectionWalkType::ElementEnd)
	{
		json_t* element = json->values.top();
		json->values.pop();
		json_array_append_new(json->values.top(), element);
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
		json->values.push( klass );
	}
	else if(wt == ReflectionWalkType::End)
	{
		json_t* klass = json_object();
		json_object_set_new(klass, ctx->klass->name, json->values.top());
		
		json->values.pop();
		json->values.push(klass);
	}
}

//-----------------------------------//

static void SerializeField(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;

	if(wt == ReflectionWalkType::End)
	{
		json_t* field = json->values.top();
		json->values.pop();
		json_object_set_new(json->values.top(), ctx->field->name, field);
		
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
	json->values.push(str);
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
		float& f = *vc.f;
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
		value = ConvertColor(c);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3& v = *vc.v;
		value = ConvertVector3(v);
		break;
	}
	case Primitive::Quaternion:
	{
		Quaternion& q = *vc.q;
		value = ConvertQuaternion(q);
		break;
	}
	case Primitive::Bitfield:
	{
		int32& i = *vc.bf;
		value = json_integer(i);
		break;
	}
	default:
		assert( false );
	}

	json->values.push(value);
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, json_t* value );

static void DeserializeEnum( ReflectionContext* context, json_t* value )
{
	const char* name = json_string_value(value);
	Enum* enume = (Enum*) context->enume;

	int32 enumValue = EnumGetValue(enume, name);
	FieldSet<int32>(context->field, context->object, enumValue);
}

//-----------------------------------//

#define setValue(T, val) FieldSet<T>(context->field, context->object, val);

static void DeserializePrimitive( ReflectionContext* context, json_t* value )
{
	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		assert( json_is_boolean(value) );
		bool val = json_typeof(value) == JSON_TRUE;
		setValue(bool, val);
		break;
	}
	case Primitive::Int32:
	{
		int32 val = (int32) json_integer_value(value);
		setValue(int32, val);
		break;
	}
	case Primitive::Uint32:
	{
		uint32 val = (uint32) json_integer_value(value);
		setValue(uint32, val);
		break;
	}
	case Primitive::Float:
	{
		float val = (float) json_real_value(value);
		setValue(float, val);
		break;
	}
	case Primitive::String:
	{
		String val = json_string_value(value);
		setValue(String, val);
		break;
	}
	case Primitive::Color:
	{
		Color val = ConvertValueToColor(value);
		setValue(Color, val);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3 val = ConvertValueToVector3(value);
		setValue(Vector3, val);
		break;
	}
	case Primitive::Quaternion:
	{
		Quaternion val = ConvertValueToQuaternion(value);
		setValue(Quaternion, val);
		break;
	}
	case Primitive::Bitfield:
	{
		int32 val = json_integer_value(value);
		setValue(int32, val);
		break;
	}
	default:
		assert(0 && "Unknown primitive type");
	}
}

//-----------------------------------//

typedef std::vector<std::shared_ptr<Object>> ObjectSharedPtrArray;
typedef std::vector<RefPtr<ReferenceCounted>> ObjectRefPtrArray;
typedef std::vector<Object*> ObjectRawPtrArray;

static void* ResizeArray( ReflectionContext* context, void* address, uint32 size )
{
	const Field* field = context->field;

	if( FieldIsSharedPointer(field) )
	{
		ObjectSharedPtrArray* array = (ObjectSharedPtrArray*) address;
		array->resize(size);
		return &array->front();
	}
	else if( FieldIsRefPointer(field) )
	{
		ObjectRefPtrArray* array = (ObjectRefPtrArray*) address;
		array->resize(size);
		return &array->front();
	}
	else if( FieldIsRawPointer(field) )
	{
		ObjectRawPtrArray* array = (ObjectRawPtrArray*) address;
		array->resize(size);
		return &array->front();
	}

	return nullptr;
}

//-----------------------------------//

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
		
		if( FieldIsRawPointer(field) )
		{
			Object** objectptr = (Object**) address;
			*objectptr = object;
		}
		else if( FieldIsSharedPointer(field) )
		{
			std::shared_ptr<Object>* shared_obj = (std::shared_ptr<Object>*) address;
			shared_obj->reset(object);
		}
		else if( FieldIsRefPointer(field) )
		{
			Object* ref = (Object*) object;
			RefPtr<Object>* ref_obj = (RefPtr<Object>*) address;
			ref_obj->reset(ref);
		}
		else
		{
			assert(0 && "Not implemented");
			//memcpy(element, object, size);
		}

		break;
	} }
}

//-----------------------------------//

static uint16 GetArrayElementSize(const Field* field)
{
	if( FieldIsPointer(field) )
		return field->pointer_size;
	else
		return field->size;
}

//-----------------------------------//

static void DeserializeArray( ReflectionContext* context, json_t* value )
{
	assert( json_is_array(value) );
	
	size_t size = json_array_size(value);
	if( size == 0 ) return;

	const Field* field = context->field;
	uint16 elementSize = GetArrayElementSize(field);

	void* address = ClassGetFieldAddress(context->object, field);
	void* begin = ResizeArray(context, address, size);

	for( size_t i = 0; i < size; i++ )
	{
		json_t* arrayValue = json_array_get(value, i);
		
		// Calculate the address of the next array element.
		void* element = (byte*) begin + elementSize * i;
		DeserializeArrayElement(context, arrayValue, element);
	}
}

//-----------------------------------//

static void DeserializeField( ReflectionContext* context, json_t* value )
{
	if( FieldIsArray(context->field) && json_is_array(value) )
	{
		DeserializeArray(context, value);
		return;
	}
	
	switch(context->field->type->type)
	{
	case Type::Composite:
	{
		Class* composite = context->composite;
		context->composite = (Class*) context->field->type;
		DeserializeComposite(context, value);
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
	} };
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
		Field* field = context->field;

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

	assert( json_object_size(value) == 1 );
	void* iter = json_object_iter(value);
	const char* key = json_object_iter_key(iter);
	json_t* iterValue = json_object_iter_value(iter);

	Class* newClass = (Class*) ReflectionFindType(key);
	if( !newClass ) return 0;

	Object* newObject = (Object*) ClassCreateInstance(newClass, json->alloc);

	Class* klass = context->klass;
	Class* composite = context->composite;
	Object* object = context->object;

	context->klass = newClass;
	context->composite = newClass;
	context->object = newObject;

	DeserializeFields(context, iterValue);

	if( ClassInherits(newClass, ReflectionGetType(Object)) )
		newObject->fixUp();

	context->object = object;
	context->composite = composite;
	context->klass = klass;

	return newObject;
}

//-----------------------------------//

static void SerializeLoad( Serializer* serializer )
{
	SerializerJSON* json = (SerializerJSON*) serializer;

	String text;
	StreamReadString(json->stream, text);
	StreamClose(json->stream);

	json_t* rootValue = nullptr;

	LocaleSwitch locale;
	
	rootValue = json_loads(text.c_str(), 0, nullptr);
	if( !rootValue ) return;

	json->rootValue = rootValue;
	
	ReflectionContext* context = &serializer->deserializeContext;
	Object* object = DeserializeComposite(context, rootValue);

	json_decref(rootValue);
	serializer->object = object;
}

//-----------------------------------//

static void SerializeSave( Serializer* serializer )
{
	SerializerJSON* json = (SerializerJSON*) serializer;

	ReflectionWalk(json->object, &json->serializeContext);
	assert( json->values.size() == 1 );
	
	json_t* rootValue = json->values.top();

	// Always switch to the platform independent "C" locale when writing
	// JSON, else the library will format the data erroneously.
	LocaleSwitch locale;

	size_t flags = JSON_INDENT(4) | JSON_PRESERVE_ORDER;
	
	char* dump = json_dumps(rootValue, flags);
	
	if( dump )
		StreamWriteString(json->stream, dump);
	
	StreamClose(json->stream);

	free(dump);
	json_decref(rootValue);
}

//-----------------------------------//

Serializer* SerializerCreateJSON(Allocator* alloc)
{
	//json_set_alloc_funcs(JsonAllocate, JsonDeallocate);

	SerializerJSON* serializer = Allocate(SerializerJSON, alloc);
	serializer->load = SerializeLoad;
	serializer->save = SerializeSave;

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

NAMESPACE_END

#endif