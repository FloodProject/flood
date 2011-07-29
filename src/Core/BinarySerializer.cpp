/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION_BINARY

#include "Core/Serialization.h"
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

#include <stack>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Vector3 ConvertValueToVector3( msgpack_object* value )
{
	msgpack_object_array& array = value->via.array;
	assert( array.size == 3 );

	float* f = (float*) array.ptr;
	
	Vector3 v;
	v.x = f[0];
	v.y = f[1];
	v.z = f[2];

	return v;
}

//-----------------------------------//

static Color ConvertValueToColor( msgpack_object* value )
{
	msgpack_object_array& array = value->via.array;
	assert( array.size == 4 );

	float* f = (float*) array.ptr;
	
	Color c;
	c.r = f[0];
	c.g = f[1];
	c.b = f[2];
	c.a = f[3];

	return c;
}

//-----------------------------------//

static Quaternion ConvertValueToQuaternion( msgpack_object* value )
{
	msgpack_object_array& array = value->via.array;
	assert( array.size == 4 );

	float* f = (float*) array.ptr;

	Quaternion q;
	q.x = f[0];
	q.y = f[1];
	q.z = f[2];
	q.w = f[3];

	return q;
}

//-----------------------------------//
#if 0
static EulerAngles ConvertValueToEulerAngles( msgpack_object* value )
{

	EulerAngles a;
	json_unpack(value, "[g,g,g]", &a.x, &a.y, &a.z);
	return a;
}

//-----------------------------------//

static msgpack_object* ConvertVector3( const Vector3& vec )
{
	return json_pack("[f,f,f]", vec.x, vec.y, vec.z);
}

//-----------------------------------//

static msgpack_object* ConvertColor( const Color& c )
{
	return json_pack("[f,f,f,f]", c.r, c.g, c.b, c.a);
}

//-----------------------------------//

static msgpack_object* ConvertQuaternion( const Quaternion& q )
{
	return json_pack("[f,f,f,f]", q.x, q.y, q.z, q.w);
}
#endif
//-----------------------------------//

struct API_CORE SerializerBinary : public Serializer
{
	msgpack_object* rootValue;
	
	// Stack of values.
	std::stack<msgpack_object*> values;
	std::stack<msgpack_object*> arrays;
};

//-----------------------------------//
#if 0
static void SerializeArray(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;

	if(wt == ReflectionWalkType::Begin)
	{
		msgpack_object* array = json_array();	
		bin->values.push( array );
	}
	else if(wt == ReflectionWalkType::ElementEnd)
	{
		msgpack_object* element = bin->values.top();
		bin->values.pop();
		json_array_append_new(bin->values.top(), element);
	}
}
#endif
//-----------------------------------//

static void SerializeComposite(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;
	
	if(wt == ReflectionWalkType::Begin)
	{
		msgpack_object* object = Allocate(msgpack_object, bin->alloc);
		object->type = MSGPACK_OBJECT_MAP;
		
		bin->values.push( object );
	}
	else if(wt == ReflectionWalkType::End)
	{
		msgpack_object* object = bin->values.top();

		//msgpack_object* object = json_object();
		//json_object_set_new(klass, ctx->klass->name, bin->values.top());
		
		bin->values.pop();
		//bin->values.push(klass);

		Deallocate(object);

	}
}

//-----------------------------------//

static void SerializeField(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;

	if(wt == ReflectionWalkType::End)
	{
		msgpack_object* field = bin->values.top();
		bin->values.pop();
		
		return;
	}
}

//-----------------------------------//
#if 0
static void SerializeEnum(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;
	
	ValueContext& vc = ctx->valueContext;
	int32 value = *vc.i32;
	
	const char* name = EnumGetValueName(ctx->enume, value);
	assert( name != nullptr );

	msgpack_object* str = json_string(name);
	bin->values.push(str);
}
#endif
//-----------------------------------//

static void SerializePrimitive( ReflectionContext* context, ReflectionWalkType::Enum wt )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	ValueContext& vc = context->valueContext;

	msgpack_object* value = Allocate(msgpack_object, bin->alloc);

	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		bool& b = *vc.b;
		value->type = MSGPACK_OBJECT_BOOLEAN;
		value->via.boolean = b;
		break;
	}
	case Primitive::Int32:
	{
		int32& i = *vc.i32;
		value->type = MSGPACK_OBJECT_NEGATIVE_INTEGER;
		value->via.i64 = i;
		break;
	}
	case Primitive::Uint32:
	{
		uint32& i = *vc.u32;
		value->type = MSGPACK_OBJECT_POSITIVE_INTEGER;
		value->via.i64 = i;
		break;
	}
	case Primitive::Float:
	{
		float& f = *vc.f;
		value->type = MSGPACK_OBJECT_DOUBLE;
		value->via.dec = f;
		break;
	}
	case Primitive::String:
	{
		String& s = *vc.s;
		//value->type = MSGPACK_OBJECT_
		//value->via.dec = f;
		break;
	}
	case Primitive::Color:
	{
		Color& c = *vc.c;
		//value = ConvertColor(c);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3& v = *vc.v;
		//value = ConvertVector3(v);
		break;
	}
	case Primitive::Quaternion:
	{
		Quaternion& q = *vc.q;
		//value = ConvertQuaternion(q);
		break;
	}
	case Primitive::Bitfield:
	{
		int32& i = *vc.bf;
		//value = json_integer(i);
		break;
	}
	default:
		assert( false );
	}

	bin->values.push(value);
}
#if 0
//-----------------------------------//

static void DeserializeEnum( ReflectionContext* context, msgpack_object* value )
{
	const char* name = json_string_value(value);
	Enum* enume = (Enum*) context->enume;

	int32 enumValue = EnumGetValue(enume, name);
	FieldSet<int32>(context->field, context->object, enumValue);
}

//-----------------------------------//

#define setValue(T, val) FieldSet<T>(context->field, context->object, val);

static void DeserializePrimitive( ReflectionContext* context, msgpack_object* value )
{
	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		assert( json_is_boolean(value) );
		bool val = msgpack_objectypeof(value) == JSON_TRUE;
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

static Object* DeserializeComposite( ReflectionContext* context, msgpack_object* value );

static void DeserializeArrayElement( ReflectionContext* context, msgpack_object* value, void* address )
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
		else if( FieldIsHandle(field) )
		{
			assert(0 && "Not implemented");
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

static void DeserializeArray( ReflectionContext* context, msgpack_object* value )
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
		msgpack_object* arrayValue = json_array_get(value, i);
		
		// Calculate the address of the next array element.
		void* element = (byte*) begin + elementSize * i;
		DeserializeArrayElement(context, arrayValue, element);
	}
}

//-----------------------------------//

static void DeserializeField( ReflectionContext* context, msgpack_object* value )
{
	Field* field = context->field;

	if( FieldIsArray(field) && json_is_array(value) )
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
		
		Object* object = nullptr;
		void* address = nullptr;

		if( FieldIsHandle(field) )
		{
			if( !json_is_object(value) )
			{
				LogDebug("Can't deserialize handle '%s'", field->name);
				return;
			}

			void* iter = json_object_iter(value);
			void* iter2 = json_object_iter( json_object_iter_value(iter) );
			msgpack_object* val = json_object_iter_value(iter2);
			const char* name = json_string_value(val);

			ReflectionHandleContext handleContext;
			ReflectionFindHandleContext((Class*) field->type, handleContext);
			
			HandleId id = handleContext.deserialize(name);
			if(id == HandleInvalid) return;

			address = ClassGetFieldAddress(context->object, field);
			
			typedef Handle<Object, 0, 0> ObjectHandle;
			ObjectHandle* handleObject = (ObjectHandle*) address;
			handleObject->id = id;
			
			ReferenceAdd((Object*)HandleFind(handleContext.handles, id));
		}
		else if( FieldIsPointer(field) )
		{
			LogDebug("Deserialization of pointer field not implemented '%s'", field->name);
		}
		else
		{
			object = DeserializeComposite(context, value);
		}

		address = ClassGetFieldAddress(object, field);
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

static void DeserializeFields( ReflectionContext* context, msgpack_object* value )
{
	void* iter = json_object_iter(value);
	for(; iter; iter = json_object_iter_next(value, iter))
	{
		const char* key = json_object_iter_key(iter);
		msgpack_object* val = json_object_iter_value(iter);

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

static Object* DeserializeComposite( ReflectionContext* context, msgpack_object* value )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;

	if( !json_is_object(value) ) return 0;

	if( json_object_size(value) != 1 )
	{
		LogDebug("Invalid field '%s' from class '%s'", context->field->name, context->composite->name);
		return nullptr;
	}

	void* iter = json_object_iter(value);
	const char* key = json_object_iter_key(iter);
	msgpack_object* iterValue = json_object_iter_value(iter);

	Class* newClass = (Class*) ReflectionFindType(key);
	if( !newClass ) return 0;

	Object* newObject = (Object*) ClassCreateInstance(newClass, bin->alloc);

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
#endif

//-----------------------------------//

static void SerializeLoad( Serializer* serializer )
{
	SerializerBinary* bin = (SerializerBinary*) serializer;

#if 0
	String text;
	StreamReadString(bin->stream, text);
	StreamClose(bin->stream);

	msgpack_object* rootValue = nullptr;

	LocaleSwitch locale;
	
	rootValue = json_loads(text.c_str(), 0, nullptr);
	
	if( !rootValue )
	{
		LogError("Could not parse JSON text of '%s'", bin->stream->path.c_str());
		return;
	}

	bin->rootValue = rootValue;
	
	ReflectionContext* context = &serializer->deserializeContext;
	Object* object = DeserializeComposite(context, rootValue);

	json_decref(rootValue);
	serializer->object = object;
#endif
}

//-----------------------------------//

static void SerializeSave( Serializer* serializer )
{
	SerializerBinary* bin = (SerializerBinary*) serializer;

	msgpack_sbuffer* sbuffer = msgpack_sbuffer_new();
	msgpack_packer* packer = msgpack_packer_new(sbuffer, msgpack_sbuffer_write);

	ReflectionWalk(bin->object, &bin->serializeContext);
	assert( bin->values.size() == 1 );
	
	msgpack_object* rootValue = bin->values.top();

	bin->stream->fn->write(bin->stream, sbuffer->data, sbuffer->size);
	StreamClose(bin->stream);

	msgpack_sbuffer_free(sbuffer);
	msgpack_packer_free(packer);
}

//-----------------------------------//

Serializer* SerializerCreateBinary(Allocator* alloc)
{
	SerializerBinary* serializer = Allocate(SerializerBinary, alloc);
	serializer->load = SerializeLoad;
	serializer->save = SerializeSave;

	ReflectionContext& sCtx = serializer->serializeContext;
	sCtx.userData = serializer;
	sCtx.walkComposite = SerializeComposite;
	sCtx.walkCompositeField = SerializeField;
	sCtx.walkPrimitive = SerializePrimitive;
#if 0
	sCtx.walkArray = SerializeArray;
	sCtx.walkEnum = SerializeEnum;
#endif

	ReflectionContext& dCtx = serializer->deserializeContext;
	dCtx.userData = serializer;

	return serializer;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif