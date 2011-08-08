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
#include "Core/SerializationHelpers.h"

#include "Math/Vector.h"
#include "Math/Quaternion.h"
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
	case Primitive::Int8:
	{
		sint8& i = *vc.i8;
		EncodeVariableInteger(bin->ms, EncodeZigZag32(i));
		break;
	}
	case Primitive::Uint8:
	{
		uint8& i = *vc.u8;
		EncodeVariableInteger(bin->ms, i);
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
	case Primitive::Int8:
	{
		DecodeVariableInteger(ms, i);
		sint8 val = (sint8) DecodeZigZag32((uint32)i);
		SetFieldValue(sint8, val);
		break;
	}
	case Primitive::Uint8:
	{
		DecodeVariableInteger(ms, i);
		uint8 val = (uint8) i;
		SetFieldValue(uint8, val);
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
	Field* field = context->field;

	if( FieldIsArray(field) )
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

		Object* object = DeserializeComposite(context, 0);
		void* address = ClassGetFieldAddress(context->object, field);
		
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
		LogDebug("Deserialize: Could not create class instance");
		return nullptr;
	}
	
	// Instantiate a new class.
	if( !newObject )
		newObject = (Object*) ClassCreateInstance(newClass, bin->alloc);

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
	Object* object = DeserializeComposite(context, 0);
	
	Deallocate(bin->ms);
	return object;
}

//-----------------------------------//

static bool SerializeSave( Serializer* serializer, Object* object )
{
	SerializerBinary* bin = (SerializerBinary*) serializer;
	if( !bin->stream ) return false;

	Stream* buf = StreamCreateFromMemory(bin->alloc, 128);
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

	return serializer;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif