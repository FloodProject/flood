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
#include "Core/SerializationHelpers.h"

#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Utilities.h"
#include "Core/References.h"
#include "Core/Stream.h"
#include "Core/Log.h"

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Color.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static void SerializeArray(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;

	if(wt == ReflectionWalkType::Begin)
	{
		EncodeVariableInteger(bin->ms, ctx->arraySize);
		return;
	}
	else if(wt == ReflectionWalkType::End)
	{
		EncodeVariableInteger(bin->ms, FieldInvalid);
		return;
	}
}

//-----------------------------------//

static void SerializeComposite(ReflectionContext* ctx, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) ctx->userData;
	Class* objectClass = ctx->objectClass;

	if(wt == ReflectionWalkType::Begin)
	{
		EncodeVariableInteger(bin->ms, objectClass->id);
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
	const Field* field = ctx->field;

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
	int32& val = vc.i32;
	
	EncodeVariableInteger(bin->ms, EncodeZigZag32(val));
}

//-----------------------------------//

static void SerializePrimitive(ReflectionContext* context, ReflectionWalkType::Enum wt)
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	ValueContext& vc = context->valueContext;

	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		bool& b = vc.b;
		EncodeVariableInteger(bin->ms, b);
		break;
	}
	case Primitive::Int8:
	{
		sint8& i = vc.i8;
		EncodeVariableInteger(bin->ms, EncodeZigZag32(i));
		break;
	}
	case Primitive::Uint8:
	{
		uint8& i = vc.u8;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Int16:
	{
		sint16& i = vc.i16;
		EncodeVariableInteger(bin->ms, EncodeZigZag32(i));
		break;
	}
	case Primitive::Uint16:
	{
		uint16& i = vc.u16;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Int32:
	{
		sint32& i = vc.i32;
		EncodeVariableInteger(bin->ms, EncodeZigZag32(i));
		break;
	}
	case Primitive::Uint32:
	{
		uint32& i = vc.u32;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Int64:
	{
		sint64& i = vc.i64;
		EncodeVariableInteger(bin->ms, EncodeZigZag64(i));
		break;
	}
	case Primitive::Uint64:
	{
		uint64& i = vc.u64;
		EncodeVariableInteger(bin->ms, i);
		break;
	}
	case Primitive::Float:
	{
		float& f = vc.f32;
		EncodeFloat(bin->ms, f);
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
		ColorP& c = vc.c;
		EncodeFloat(bin->ms, c.r);
		EncodeFloat(bin->ms, c.g);
		EncodeFloat(bin->ms, c.b);
		EncodeFloat(bin->ms, c.a);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3P& v = vc.v;
		EncodeFloat(bin->ms, v.x);
		EncodeFloat(bin->ms, v.y);
		EncodeFloat(bin->ms, v.z);
		break;
	}
	case Primitive::Quaternion:
	{
		QuaternionP& q = vc.q;
		EncodeFloat(bin->ms, q.x);
		EncodeFloat(bin->ms, q.y);
		EncodeFloat(bin->ms, q.z);
		EncodeFloat(bin->ms, q.w);
		break;
	}
	default:
		assert( false );
	}
}

//-----------------------------------//

static void DeserializeEnum( ReflectionContext* context, ReflectionWalkType::Enum = ReflectionWalkType::End )
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

#define SetFieldValue(T, val) \
	MULTI_LINE_MACRO_BEGIN \
	if(context->field) \
		FieldSet<T>(context->field, context->object, val); \
	MULTI_LINE_MACRO_END

static void DeserializePrimitive( ReflectionContext* context, ReflectionWalkType::Enum = ReflectionWalkType::End )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	MemoryStream* ms = bin->ms;
	uint64 i;

	ValueContext& vc = context->valueContext;

	switch(context->primitive->type)
	{
	case Primitive::Bool:
	{
		DecodeVariableInteger(ms, i);
		vc.b = i != 0;
		SetFieldValue(bool, vc.b);
		break;
	}
	case Primitive::Int8:
	{
		DecodeVariableInteger(ms, i);
		vc.i8 = (sint8) DecodeZigZag32((uint32)i);
		SetFieldValue(sint8, vc.i8);
		break;
	}
	case Primitive::Uint8:
	{
		DecodeVariableInteger(ms, i);
		vc.u8 = (uint8) i;
		SetFieldValue(uint8, vc.u8);
		break;
	}
	case Primitive::Int16:
	{
		DecodeVariableInteger(ms, i);
		vc.i16 = (sint16) DecodeZigZag32((uint32)i);
		SetFieldValue(sint16, vc.i16);
		break;
	}
	case Primitive::Uint16:
	{
		DecodeVariableInteger(ms, i);
		vc.u16 = (uint16) i;
		SetFieldValue(uint16, vc.u16);
		break;
	}
	case Primitive::Int32:
	{
		DecodeVariableInteger(ms, i);
		vc.i32 = (sint32) DecodeZigZag32((uint32)i);
		SetFieldValue(sint32, vc.i32);
		break;
	}
	case Primitive::Uint32:
	{
		DecodeVariableInteger(ms, i);
		vc.u32 = (uint32) i;
		SetFieldValue(uint32, vc.u32);
		break;
	}
	case Primitive::Int64:
	{
		DecodeVariableInteger(ms, i);
		vc.i64 = (sint64) DecodeZigZag64(i);
		SetFieldValue(sint64, vc.i64);
		break;
	}
	case Primitive::Uint64:
	{
		DecodeVariableInteger(ms, i);
		vc.u64 = i;
		SetFieldValue(uint64, vc.u64);
		break;
	}
	case Primitive::Float:
	{
		vc.f32 = DecodeFloat(ms);
		SetFieldValue(float, vc.f32);
		break;
	}
	case Primitive::Color:
	{
		ColorP& val = vc.c;
		val.r = DecodeFloat(ms);
		val.g = DecodeFloat(ms);
		val.b = DecodeFloat(ms);
		val.a = DecodeFloat(ms);
		SetFieldValue(ColorP, val);
		break;
	}
	case Primitive::Vector3:
	{
		Vector3P& val = vc.v;
		val.x = DecodeFloat(ms);
		val.y = DecodeFloat(ms);
		val.z = DecodeFloat(ms);
		SetFieldValue(Vector3P, val);
		break;
	}
	case Primitive::Quaternion:
	{
		QuaternionP& val = vc.q;
		val.x = DecodeFloat(ms);
		val.y = DecodeFloat(ms);
		val.z = DecodeFloat(ms);
		val.w = DecodeFloat(ms);
		SetFieldValue(QuaternionP, val);
		break;
	}
	case Primitive::String:
	{
		String val;
		DecodeString(ms, val);
		SetFieldValue(String, val);
		break;
	}
	default:
		assert(0 && "Unknown primitive type");
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, Object* newObject );

static void DeserializeArrayElement( ReflectionContext* context, void* address )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	const Field* field = context->field;

	switch(field->type->type)
	{
	case Type::Primitive:
	{
		LogAssert("Not implemented");
		break;
	}
	case Type::Enumeration:
	{
		LogAssert("Not implemented");
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
}

//-----------------------------------//

static void DeserializeArray( ReflectionContext* context )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;
	const Field* field = context->field;
	
	uint64 size;
	DecodeVariableInteger(bin->ms, size);

	if(size == 0) return;

	uint16 elementSize = ReflectionArrayGetElementSize(field);
	void* address = ClassGetFieldAddress(context->object, field);
	void* begin = ReflectionArrayResize(context, address, size);

	uint32 n = 0;
	while( n < size )
	{
		// Calculate the address of the next array element.
		void* element = (byte*) begin + elementSize * n++;
		DeserializeArrayElement(context, element);
	}

	uint64 end;
	DecodeVariableInteger(bin->ms, end);
	
	if( end != FieldInvalid )
	{
		LogAssert("Expected end of array");
		return;
	}
}

//-----------------------------------//

static void DeserializeField( ReflectionContext* context )
{
	const Field* field = context->field;

	if( field->serialize )
	{
		field->serialize(context, ReflectionWalkType::Element);
		return;
	}
	else if( FieldIsArray(field) )
	{
		DeserializeArray(context);
		return;
	}
	
	switch(field->type->type)
	{
	case Type::Composite:
	{
		Class* composite = context->composite;
		context->composite = (Class*) field->type;

		void* address = ClassGetFieldAddress(context->object, field);
		Object* store = FieldIsPointer(field) ? 0 : (Object*) address;

		Object* object = DeserializeComposite(context, store);
		
		if( FieldIsHandle(field) )
		{
			LogDebug("Deserialization of handles not implemented '%s'", field->name);
		}
		else if( FieldIsPointer(field) )
		{
			PointerSetObject(field, address, object);
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

void DeserializeFields( ReflectionContext* context )
{
	SerializerBinary* bin = (SerializerBinary*) context->userData;

	uint64 val;
	while( DecodeVariableInteger(bin->ms, val) )
	{
		FieldId id = (FieldId) val;
		
		// This marks the end of the composite.
		if(id == FieldInvalid) break;

		Class* composite = context->composite;
		Field* newField = ClassGetFieldById(composite, id);
		
		if( !newField )
		{
			LogDebug("Unknown field '%d' of class '%s'", id, composite->name);
			continue;
		}
		
		const Field* field = context->field;
		context->field = newField;

		DeserializeField(context);

		context->field = field;
		context->composite = composite;
	}
}

//-----------------------------------//

static Object* DeserializeComposite( ReflectionContext* context, Object* newObject )
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
		LogDebug("Deserialize: Invalid class id");
		return nullptr;
	}

	Class* newClass = it->second;
	
	if( !newClass )
	{
		LogDebug("Deserialize: Invalid class id");
		return nullptr;
	}

#if 0
	Class* fieldClass = (Class*) context->field;

	if( fieldClass && !ClassInherits(fieldClass, newClass) )
		return 0;
#endif

	// Instantiate an instance of the class.
	if( !newObject )
		newObject = (Object*) ClassCreateInstance(newClass, bin->alloc);

	if( !newObject ) return 0;

	Class* objectClass = context->objectClass;
	Class* composite = context->composite;
	Object* object = context->object;

	context->objectClass = newClass;
	context->composite = newClass;
	context->object = newObject;

	if( newClass->serialize )
	{
		newClass->serialize(context, ReflectionWalkType::Begin);

		// This reads the end marker of the class.
		DeserializeFields(context);
	}
	else
		DeserializeFields(context);

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
	SerializerBinary* bin = (SerializerBinary*) serializer;
	if( !bin->stream ) return nullptr;

	int64 size = StreamGetSize(bin->stream);
	if( size == 0 ) return nullptr;

	bin->ms = (MemoryStream*) StreamCreateFromMemory(bin->alloc, size);
	StreamReadBuffer(bin->stream, &bin->ms->data[0], size);

	ReflectionContext* context = &serializer->deserializeContext;

	Object* object = serializer->object;
	object = DeserializeComposite(context, object);
	
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
	serializer->alloc = alloc;
	
	ReflectionContext& sCtx = serializer->serializeContext;
	sCtx.userData = serializer;
	sCtx.walkComposite = SerializeComposite;
	sCtx.walkCompositeField = SerializeField;
	sCtx.walkPrimitive = SerializePrimitive;
	sCtx.walkEnum = SerializeEnum;
	sCtx.walkArray = SerializeArray;

	ReflectionContext& dCtx = serializer->deserializeContext;
	dCtx.userData = serializer;
	dCtx.walkPrimitive = DeserializePrimitive;
	dCtx.walkEnum = DeserializeEnum;

	return serializer;
}

//-----------------------------------//

#ifdef BUILD_DEBUG

void StreamAdvanceIndex(MemoryStream* ms, uint64 n)
{
	ms->position += n;

	// Do some debug bounds checking.
	if(ms->position > ms->data.size())
	{
		LogAssert("Check the bounds of the buffer");
	}
}

#endif

//-----------------------------------//

void EncodeVariableIntegerBuffer(uint8* buf, uint64& advance, uint64 val)
{
	do
	{
		uint8 byte = val & 0x7f;
		val >>= 7;
		if(val) byte |= 0x80;
		*buf++ = byte;
		advance++;
	}
	while(val);
}

void EncodeVariableInteger(MemoryStream* ms, uint64 val)
{
	uint8* buf = StreamIndex(ms);
	EncodeVariableIntegerBuffer(buf, ms->position, val);
}

bool DecodeVariableIntegerBuffer(uint8* buf, uint64& advance, uint64& val)
{
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

	advance += p-buf;
	val = ((uint64) high << 32) | low;
	return true;
}

bool DecodeVariableInteger(MemoryStream* ms, uint64& val)
{
	uint8* buf = StreamIndex(ms);
	return DecodeVariableIntegerBuffer(buf, ms->position, val);
}

//-----------------------------------//

/**
 * For signed numbers, we use an encoding called zig-zag encoding
 * that maps signed numbers to unsigned numbers so they can be
 * efficiently encoded using the normal variable-int encoder.
 */

uint32 EncodeZigZag32(sint32 n)
{
	return (n << 1) ^ (n >> 31);
}

sint32 DecodeZigZag32(uint32 n)
{
	return (n >> 1) ^ -(sint32)(n & 1);
}

uint64 EncodeZigZag64(sint64 n)
{
	return (n << 1) ^ (n >> 63);
}

sint64 DecodeZigZag64(uint64 n)
{
	return (n >> 1) ^ -(sint64)(n & 1);
}

//-----------------------------------//

void EncodeFixed32(MemoryStream* ms, uint32 val)
{
	uint8* buf = StreamIndex(ms);
	buf[0] = val & 0xff;
	buf[1] = (val >> 8) & 0xff;
	buf[2] = (val >> 16) & 0xff;
	buf[3] = (val >> 24);
	StreamAdvanceIndex(ms, sizeof(uint32));
}

uint32 DecodeFixed32(MemoryStream* ms)
{
	uint8* buf = StreamIndex(ms);
	uint32* val = (uint32*) buf;
	StreamAdvanceIndex(ms, sizeof(uint32));
	return *val;
}

//-----------------------------------//

void EncodeFloat(MemoryStream* ms, float val)
{
	uint32* p = (uint32*) &val;
	EncodeFixed32(ms, *p);
}

float DecodeFloat(MemoryStream* ms)
{
	uint32 val = DecodeFixed32(ms);
	float* p = (float*) &val;
	return *p;
}

//-----------------------------------//

void EncodeString(MemoryStream* ms, const String& s)
{
	EncodeVariableInteger(ms, s.size());
	StreamWriteString(ms, s);
}

bool DecodeString(MemoryStream* ms, String& s)
{
	uint64 size;
	
	if( !DecodeVariableInteger(ms, size) )
		return false;

	s.resize((size_t)size);
	memcpy((void*) s.data(), StreamIndex(ms), (size_t)size);

	StreamAdvanceIndex(ms, size);
	return true;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif