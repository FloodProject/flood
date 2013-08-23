/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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

#include "Core/Math/Vector.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/EulerAngles.h"
#include "Core/Math/Color.h"

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
	int r, g, b, a;
	json_unpack(value, "[i,i,i,i]", &r, &g, &b, &a);

	return Color(byte(r), byte(g), byte(b), byte(a));
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

ValueContext ConvertValueToPrimitive( PrimitiveTypeKind kind, json_t* value )
{
	ValueContext vc;

	switch(kind)
	{
	case PrimitiveTypeKind::Bool:
	{
		assert( json_is_boolean(value) );
		vc.b = json_typeof(value) == JSON_TRUE;
		break;
	}
	case PrimitiveTypeKind::Int16:
	{
		vc.i16 = (int16) json_integer_value(value);
		break;
	}
	case PrimitiveTypeKind::Uint16:
	{
		vc.u16 = (uint16) json_integer_value(value);
		break;
	}
	case PrimitiveTypeKind::Int32:
	{
		vc.i32 = (int32) json_integer_value(value);
		break;
	}
	case PrimitiveTypeKind::Uint32:
	{
		vc.u32 = (uint32) json_integer_value(value);
		break;
	}
	case PrimitiveTypeKind::Int64:
	{
		vc.i64 = (int64) json_integer_value(value);
		break;
	}
	case PrimitiveTypeKind::Uint64:
	{
		vc.u64 = (uint64) json_integer_value(value);
		break;
	}
	case PrimitiveTypeKind::Float:
	{
		vc.f32 = (float) json_real_value(value);
		break;
	}
	case PrimitiveTypeKind::String:
	{
		vc.cs = json_string_value(value);
		break;
	}
	case PrimitiveTypeKind::Color:
	{
		vc.c = ConvertValueToColor(value);
		break;
	}
	case PrimitiveTypeKind::Vector3:
	{
		vc.v = ConvertValueToVector3(value);
		break;
	}
	case PrimitiveTypeKind::Quaternion:
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

static void SerializeArray(ReflectionContext* ctx, ReflectionWalkType wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;

	if(wt == ReflectionWalkType::Begin)
	{
		json_t* array = json_array();	
		json->values.pushBack( array );
	}
	else if(wt == ReflectionWalkType::ElementEnd)
	{
		json_t* element = json->values.back();
		json->values.popBack();
		json_array_append_new(json->values.back(), element);
	}
}

//-----------------------------------//

// A composite object is serialized via two JSON objects. The first object has just
// one key and contains the name of the concrete class of the type. This is used
// to handle polymorphism, where a derived instance is used where a base is expected.

static void SerializeComposite(ReflectionContext* ctx, ReflectionWalkType wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;
	
	if(wt == ReflectionWalkType::Begin)
	{
		json_t* klass = json_object();
		json->values.pushBack( klass );
	}
	else if(wt == ReflectionWalkType::End)
	{
		json_t* klass = json_object();
		json_object_set_new(klass, ctx->objectClass->name, json->values.back());
		
		json->values.popBack();
		json->values.pushBack(klass);
	}
}

//-----------------------------------//

static void SerializeField(ReflectionContext* ctx, ReflectionWalkType wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;

	if(wt == ReflectionWalkType::End)
	{
		json_t* field = json->values.back();
		json->values.popBack();
		json_object_set_new(json->values.back(), ctx->field->name, field);
		
		return;
	}
}

//-----------------------------------//
	
static void SerializeEnum(ReflectionContext* ctx, ReflectionWalkType wt)
{
	SerializerJSON* json = (SerializerJSON*) ctx->userData;
	
	ValueContext& vc = ctx->valueContext;
	const char* name = EnumGetValueName(ctx->enume, vc.i32);
	assert( name != nullptr );

	json_t* str = json_string(name);
	json->values.pushBack(str);
}

//-----------------------------------//

static void SerializePrimitive( ReflectionContext* context, ReflectionWalkType wt )
{
	SerializerJSON* json = (SerializerJSON*) context->userData;
	ValueContext& vc = context->valueContext;

	json_t* value = nullptr;

	switch(context->primitive->kind)
	{
	case PrimitiveTypeKind::Bool:
	{
		bool& b = vc.b;
		value = b ? json_true() : json_false();
		break;
	}
	case PrimitiveTypeKind::Int32:
	{
		int32& i = vc.i32;
		value = json_integer(i);
		break;
	}
	case PrimitiveTypeKind::Uint32:
	{
		uint32& i = vc.u32;
		value = json_integer(i);
		break;
	}
	case PrimitiveTypeKind::Float:
	{
		float& f = vc.f32;
		value = json_real(f);
		break;
	}
	case PrimitiveTypeKind::String:
	{
		String& s = *vc.s;
		value = json_string( s.c_str() );
		break;
	}
	case PrimitiveTypeKind::Color:
	{
		ColorP& c = vc.c;
		value = json_pack("[i,i,i,i]", c.r, c.g, c.b, c.a);
		break;
	}
	case PrimitiveTypeKind::Vector3:
	{
		Vector3P& v = vc.v;
		value = json_pack("[f,f,f]", v.x, v.y, v.z);
		break;
	}
	case PrimitiveTypeKind::Quaternion:
	{
		QuaternionP& q = vc.q;
		value = json_pack("[f,f,f,f]", q.x, q.y, q.z, q.w);
		break;
	}
	default:
		assert( false );
	}

	json->values.pushBack(value);
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

	ValueContext vc = ConvertValueToPrimitive(context->primitive->kind, value);

	switch(context->primitive->kind)
	{
	case PrimitiveTypeKind::Bool: { SetFieldValue(bool, vc.b); break; }
	case PrimitiveTypeKind::Int32: { SetFieldValue(int32, vc.i32); break; }
	case PrimitiveTypeKind::Uint32: { SetFieldValue(uint32, vc.u32); break; }
	case PrimitiveTypeKind::Float: { SetFieldValue(float, vc.f32); break; }
	case PrimitiveTypeKind::String: { SetFieldValue(String, vc.cs); break; }
	case PrimitiveTypeKind::Color: { SetFieldValue(ColorP, vc.c); break; }
	case PrimitiveTypeKind::Vector3: { SetFieldValue(Vector3P, vc.v); break; }
	case PrimitiveTypeKind::Quaternion: { SetFieldValue(QuaternionP, vc.q); break; }
	default: assert(0 && "Unknown primitive type kind");
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, Object* newObject );

static void DeserializeArrayElement( ReflectionContext* context, void* address )
{
	const Field* field = context->field;

	switch(field->type->kind)
	{
	case TypeKind::Primitive:
	{
		assert(0 && "Not implemented");
		break;
	}
	case TypeKind::Enumeration:
	{
		assert(0 && "Not implemented");
		break;
	}
	case TypeKind::Composite:
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
	json->values.popBack();
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
			json->values.pushBack(arrayValue);

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

			json->values.pushBack(arrayValue);

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
	ReflectionFindHandleContext(context->handleContextMap, (Class*) field->type,
		handleContext);
	
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
	while( (iter2 = json_object_iter_next(obj, iter2)) );
}

//-----------------------------------//

static void DeserializeField( ReflectionContext* context, ReflectionWalkType wt )
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

	switch(field->type->kind)
	{
	case TypeKind::Composite:
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
	case TypeKind::Primitive:
	{
		context->primitive = (Primitive*) context->field->type;
		DeserializePrimitive(context);
		break;
	}
	case TypeKind::Enumeration:
	{
		context->enume = (Enum*) context->field->type;
		DeserializeEnum(context);
		break;
	} }
}

//-----------------------------------//

static void DeserializeFields( ReflectionContext* context, ReflectionWalkType )
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

		json->values.pushBack(fieldValue);
		DeserializeField(context, ReflectionWalkType::Element);
		json->values.popBack();

		context->field = field;
		context->composite = composite;
	}
}

//-----------------------------------//

static Object* DeserializeComposite(ReflectionContext* context, Object* newObject)
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
		newObject = (Object*) ClassCreateInstance(newClass, json->allocator);

	if( !newObject ) return 0;

	Class* objectClass = context->objectClass;
	Class* composite = context->composite;
	Object* object = context->object;

	context->objectClass = newClass;
	context->composite = newClass;
	context->object = newObject;

	json->values.pushBack(value);

	if( newClass->serialize )
		newClass->serialize(context, ReflectionWalkType::Begin);
	else
		DeserializeFields(context, ReflectionWalkType::Begin);

	json->values.popBack();

	if( ClassInherits(newClass, ReflectionGetType(Object)) )
		newObject->fixUp();

	context->object = object;
	context->composite = composite;
	context->objectClass = objectClass;

	return newObject;
}

//-----------------------------------//

Object* SerializerJSON::load()
{
	rootValue = nullptr;
	values.clear();

	String text;
	stream->readString(text);
	stream->close();

	json_t* _rootValue;

	LocaleSwitch locale;
	_rootValue = json_loads(text.c_str(), 0, nullptr);
	
	if (!_rootValue)
	{
		LogError("Could not parse JSON text of '%s'", stream->path.c_str());
		return nullptr;
	}

	rootValue = _rootValue;
	
	ReflectionContext* context = &deserializeContext;

	json->values.pushBack(rootValue);

	object = DeserializeComposite(context, object);

	json->values.popBack();
	assert( json->values.empty() );

	json_decref(rootValue);

	stream->close();

	return object;
}

//-----------------------------------//

bool SerializerJSON::save(const Object* obj)
{
	this->rootValue = nullptr;
	values.clear();
	object = const_cast<Object *>(obj);

	ReflectionWalk(object, &serializeContext);
	assert( values.size() == 1 );
		
	json_t* rootValue = values.back();

	// Always switch to the platform independent "C" locale when writing
	// JSON, else the library will format the data erroneously.

	LocaleSwitch locale;

	size_t flags = JSON_INDENT(4) | JSON_PRESERVE_ORDER;
	
	char* dump = json_dumps(rootValue, flags);
	
	if (dump)
		stream->writeString(dump);
	
	stream->close();

	Deallocate(dump);
	json_decref(rootValue);

	object = nullptr;
	
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

SerializerJSON::SerializerJSON(Allocator* alloc, ReflectionHandleContextMap* handleContextMap)
	: Serializer(alloc)
{
	#pragma TODO("Hook memory allocators to JSON library")
	//json_set_alloc_funcs(JsonAllocate, JsonDeallocate);

	ReflectionContext& sCtx = serializeContext;
	sCtx.userData = this;
	sCtx.walkArray = SerializeArray;
	sCtx.walkComposite = SerializeComposite;
	sCtx.walkCompositeField = SerializeField;
	sCtx.walkPrimitive = SerializePrimitive;
	sCtx.walkEnum = SerializeEnum;
	sCtx.handleContextMap = handleContextMap;

	ReflectionContext& dCtx = deserializeContext;
	dCtx.userData = this;
	dCtx.walkCompositeFields = DeserializeFields;
	dCtx.walkCompositeField = DeserializeField;
	sCtx.handleContextMap = handleContextMap;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif