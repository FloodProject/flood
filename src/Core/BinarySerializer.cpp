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

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct API_CORE SerializerBinary : public Serializer
{
	MemoryStream* ms;
	uint8* buf;
	uint16 arraySize;
};

//-----------------------------------//

/**
 * Wire format is inspired on Google Protocol Buffers.
 * http://code.google.com/apis/protocolbuffers/docs/encoding.html
 */

#define StreamIndex(s) ((&s->data[0])+s->position)
#define StreamAdvanceIndex(s, n) (s->position += n)

static void EncodeVariableInteger(MemoryStream* ms, uint64 val)
{
	uint8 n = 0;
	uint8* buf = StreamIndex(ms);

	do
	{
		uint8 byte = val & 0x7f;
		val >>= 7;
		if(val) byte |= 0x80;
		*buf++ = byte;
		n++;
	}
	while(val);

	StreamAdvanceIndex(ms, n);
}

static bool DecodeVariableInteger(MemoryStream* ms, uint64& val)
{
	uint8* buf = StreamIndex(ms);
	uint8* p = buf;

	uint32 low, high = 0;
	uint32 b;
	b = *(p++); low   = (b & 0x7f)      ; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) <<  7; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) << 14; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) << 21; if(!(b & 0x80)) goto done;
	b = *(p++); low  |= (b & 0x7f) << 28;
				high  = (b & 0x7f) >>  4; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) <<  3; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 10; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 17; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 24; if(!(b & 0x80)) goto done;
	b = *(p++); high |= (b & 0x7f) << 31; if(!(b & 0x80)) goto done;
	return false;

done:

	StreamAdvanceIndex(ms, p-buf);
	val = ((uint64) high << 32) | low;
	return true;
}

//-----------------------------------//

/**
 * For signed numbers, we use an encoding called zig-zag encoding
 * that maps signed numbers to unsigned numbers so they can be
 * efficiently encoded using the normal variable-int encoder.
 */

static uint32 EncodeZigZag32(sint32 n)
{
	return (n << 1) ^ (n >> 31);
}

static sint32 DecodeZigZag32(uint32 n)
{
	return (n >> 1) ^ -(sint32)(n & 1);
}

static uint64 EncodeZigZag64(sint64 n)
{
	return (n << 1) ^ (n >> 63);
}

static sint64 DecodeZigZag64(uint64 n)
{
	return (n >> 1) ^ -(sint64)(n & 1);
}

//-----------------------------------//

static void EncodeFixed32(MemoryStream* ms, uint32 val)
{
	uint8* buf = StreamIndex(ms);
	buf[0] = val & 0xff;
	buf[1] = (val >> 8) & 0xff;
	buf[2] = (val >> 16) & 0xff;
	buf[3] = (val >> 24);
	StreamAdvanceIndex(ms, sizeof(uint32));
}

static uint32 DecodeFixed32(MemoryStream* ms)
{
	uint8* buf = StreamIndex(ms);
	uint32* val = (uint32*) buf;
	StreamAdvanceIndex(ms, sizeof(uint32));
	return *val;
}

//-----------------------------------//

static void EncodeString(MemoryStream* ms, const String& s)
{
	EncodeVariableInteger(ms, s.size());
	StreamWriteString(ms, s);
}

static bool DecodeString(MemoryStream* ms, String& s)
{
	uint64 size;
	
	if( !DecodeVariableInteger(ms, size) )
		return false;

	s.resize((size_t)size);
	memcpy(&s[0], StreamIndex(ms), (size_t)size);

	StreamAdvanceIndex(ms, size);

	return true;
}

//-----------------------------------//

static void SerializeArray(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;

	if(wt == ReflectionWalkType::Begin)
	{
		EncodeVariableInteger(bin->ms, ctx->arraySize);
		return;
	}
}

//-----------------------------------//

static void SerializeComposite(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;
	Class* klass = ctx->klass;

	if(wt == ReflectionWalkType::Begin)
	{
		EncodeVariableInteger(bin->ms, klass->id);
		return;
	}
	else if(wt == ReflectionWalkType::End)
	{
		EncodeVariableInteger(bin->ms, FieldInvalid);
		return;
	}
}

//-----------------------------------//

static void SerializeField(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;
	Field* field = ctx->field;

	if(wt == ReflectionWalkType::Begin)
	{
		EncodeVariableInteger(bin->ms, field->id);
		return;
	}
}

//-----------------------------------//

static void SerializeEnum(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;
	
	ValueContext& vc = ctx->valueContext;
	int32& val = *vc.i32;
	
	EncodeVariableInteger(bin->ms, EncodeZigZag32(val));
}

//-----------------------------------//

static void SerializePrimitive( ReflectionContext* context, ReflectionWalkType::Enum wt )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	ValueContext& vc = context->valueContext;

	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		bool& b = *vc.b;
		EncodeVariableInteger(bin->ms, b);
		break;
	}
	case Primitive::Int16:
	{
		sint16& i = *vc.i16;
		EncodeVariableInteger(bin->ms, EncodeZigZag32(i));
		break;
	}
	case Primitive::Uint16:
	{
		uint16& i = *vc.u16;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Int32:
	{
		sint32& i = *vc.i32;
		EncodeVariableInteger(bin->ms, EncodeZigZag32(i));
		break;
	}
	case Primitive::Uint32:
	{
		uint32& i = *vc.u32;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Int64:
	{
		sint64& i = *vc.i64;
		EncodeVariableInteger(bin->ms, EncodeZigZag64(i));
		break;
	}
	case Primitive::Uint64:
	{
		uint64& i = *vc.u64;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Float:
	{
		float& f = *vc.f;
		EncodeFixed32(bin->ms, (uint32)f);
		break;
	}
	case Primitive::String:
	{
		String& s = *vc.s;
		EncodeString(bin->ms, s);
		break;
	}
	case Primitive::Color:
	{
		Color& c = *vc.c;
		EncodeFixed32(bin->ms, (uint32) c.r);
		EncodeFixed32(bin->ms, (uint32) c.g);
		EncodeFixed32(bin->ms, (uint32) c.b);
		EncodeFixed32(bin->ms, (uint32) c.a);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3& v = *vc.v;
		EncodeFixed32(bin->ms, (uint32) v.x);
		EncodeFixed32(bin->ms, (uint32) v.y);
		EncodeFixed32(bin->ms, (uint32) v.z);
		break;
	}
	case Primitive::Quaternion:
	{
		Quaternion& q = *vc.q;
		EncodeFixed32(bin->ms, (uint32) q.x);
		EncodeFixed32(bin->ms, (uint32) q.y);
		EncodeFixed32(bin->ms, (uint32) q.z);
		EncodeFixed32(bin->ms, (uint32) q.w);
		break;
	}
	case Primitive::Bitfield:
	{
		int32& i = *vc.bf;
		EncodeFixed32(bin->ms, (uint32) i);
		break;
	}
	default:
		assert( false );
	}
}

//-----------------------------------//

static void DeserializeEnum( ReflectionContext* context )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	Enum* enume = (Enum*) context->enume;

	uint64 val;
	if( !DecodeVariableInteger(bin->ms, val) )
		return;
	
	sint32 i = DecodeZigZag32((uint32)val);
	FieldSet(context->field, context->object, i);
}

//-----------------------------------//

#define SetFieldValue(T, val) FieldSet<T>(context->field, context->object, val);

static void DeserializePrimitive( ReflectionContext* context )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	MemoryStream* ms = bin->ms;
	uint64 i;

	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		DecodeVariableInteger(ms, i);
		bool val = i != 0;
		SetFieldValue(bool, val);
		break;
	}
	case Primitive::Int16:
	{
		DecodeVariableInteger(ms, i);
		sint16 val = (sint16) DecodeZigZag32((uint32)i);
		SetFieldValue(sint16, val);
		break;
	}
	case Primitive::Uint16:
	{
		DecodeVariableInteger(ms, i);
		uint16 val = (uint16) i;
		SetFieldValue(uint16, val);
		break;
	}
	case Primitive::Int32:
	{
		DecodeVariableInteger(ms, i);
		sint32 val = (sint32) DecodeZigZag32((uint32)i);
		SetFieldValue(sint32, val);
		break;
	}
	case Primitive::Uint32:
	{
		DecodeVariableInteger(ms, i);
		uint32 val = (uint32) i;
		SetFieldValue(uint32, val);
		break;
	}
	case Primitive::Int64:
	{
		DecodeVariableInteger(ms, i);
		sint64 val = (sint64) DecodeZigZag64(i);
		SetFieldValue(sint64, val);
		break;
	}
	case Primitive::Uint64:
	{
		DecodeVariableInteger(ms, i);
		SetFieldValue(uint64, i);
		break;
	}
	case Primitive::Float:
	{
		float val = (float) DecodeFixed32(ms);
		SetFieldValue(float, val);
		break;
	}
	case Primitive::String:
	{
		String val;
		DecodeString(ms, val);
		SetFieldValue(String, val);
		break;
	}
	case Primitive::Color:
	{
		ColorP val;
		val.r = (float) DecodeFixed32(ms);
		val.g = (float) DecodeFixed32(ms);
		val.b = (float) DecodeFixed32(ms);
		val.a = (float) DecodeFixed32(ms);
		SetFieldValue(ColorP, val);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3P val;
		val.x = (float) DecodeFixed32(ms);
		val.y = (float) DecodeFixed32(ms);
		val.z = (float) DecodeFixed32(ms);
		SetFieldValue(Vector3P, val);
		break;
	}
	case Primitive::Quaternion:
	{
		QuaternionP val;
		val.x = (float) DecodeFixed32(ms);
		val.y = (float) DecodeFixed32(ms);
		val.z = (float) DecodeFixed32(ms);
		val.w = (float) DecodeFixed32(ms);
		SetFieldValue(QuaternionP, val);
		break;
	}
	case Primitive::Bitfield:
	{
		int32 val = DecodeFixed32(ms);
		SetFieldValue(int32, val);
		break;
	}
	default:
		assert(0 && "Unknown primitive type");
	}
}
#if 0
//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context );

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
		Object* object = DeserializeComposite(context, value);
		
		break;
	} }
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
#endif

//-----------------------------------//

static void DeserializeField( ReflectionContext* context )
{
	Field* field = context->field;

	if( FieldIsArray(field) )
	{
		//DeserializeArray(context);
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
#if 0
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
#endif
		}
		else if( FieldIsPointer(field) )
		{
			LogDebug("Deserialization of pointer field not implemented '%s'", field->name);
		}
		else
		{
			//object = DeserializeComposite(context);
		}

		address = ClassGetFieldAddress(object, field);
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

static void DeserializeFields( ReflectionContext* context )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;

	uint64 val;
	while( DecodeVariableInteger(bin->ms, val) )
	{
		FieldId id = (FieldId) val;
		
		// This marks the end of the composite.
		if(id == FieldInvalid) break;

		Class* composite = context->composite;
		Field* field = context->field;

		Field* newField = ClassGetFieldById(composite, id);
		
		if( !newField )
		{
			LogDebug("Unknown field '%d' of class '%s'", id, composite->name);
			continue;
		}
		
		context->field = newField;
		DeserializeField(context);

		context->field = field;
		context->composite = composite;
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context)
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	ClassIdMap& ids = ClassGetIdMap();

	// Read the class id.
	uint64 val;
	
	if( !DecodeVariableInteger(bin->ms, val) )
		return nullptr;

	ClassId id = (ClassId) val;
	
	// Find the class id.
	ClassIdMap::iterator it = ids.find(id);

	if( it == ids.end() )
	{
		LogDebug("Invalid class id");
		return nullptr;
	}

	Class* newClass = it->second;
	
	if( !newClass )
	{
		LogDebug("Invalid class");
		return nullptr;
	}
	
	// Instantiate a new class.
	Object* newObject = (Object*) ClassCreateInstance(newClass, bin->alloc);

	Class* klass = context->klass;
	Class* composite = context->composite;
	Object* object = context->object;

	context->klass = newClass;
	context->composite = newClass;
	context->object = newObject;

	DeserializeFields(context);

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
	SerializerBinary* bin = (SerializerBinary*) serializer;
	if( !bin->stream ) return nullptr;

	int64 size = StreamGetSize(bin->stream);
	bin->ms = (MemoryStream*) StreamCreateFromMemory(bin->alloc, size);
	StreamReadBuffer(bin->stream, &bin->ms->data[0], size);

	ReflectionContext* context = &serializer->deserializeContext;
	Object* object = DeserializeComposite(context);
	
	Deallocate(bin->ms);
	return object;
}

//-----------------------------------//

static bool SerializeSave( Serializer* serializer, Object* object )
{
	SerializerBinary* bin = (SerializerBinary*) serializer;
	if( !bin->stream ) return false;

	Stream* buf = StreamCreateFromMemory(bin->alloc, 1024);
	bin->ms = (MemoryStream*) buf;
	
	ReflectionWalk(object, &bin->serializeContext);
	StreamWrite(bin->stream, &bin->ms->data[0], bin->ms->position);

	StreamDestroy(buf);
	StreamClose(bin->stream);

	return true;
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
	sCtx.walkEnum = SerializeEnum;
	sCtx.walkArray = SerializeArray;

	ReflectionContext& dCtx = serializer->deserializeContext;
	dCtx.userData = serializer;

	return serializer;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif