/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION

#include "Core/Serialization.h"
#include "Core/SerializationHelpers.h"
#include "Core/References.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Log.h"
#include "Core/Stream.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/EulerAngles.h"
#include "Core/Math/Color.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static void WalkNull(ReflectionContext*, ReflectionWalkType::Enum)
{
}

ReflectionContext::ReflectionContext()
	: loading(false)
	, userData(nullptr)
	, object(nullptr)
	, objectClass(nullptr)
	, type(nullptr)
	, enume(nullptr)
	, composite(nullptr)
	, primitive(nullptr)
	, field(nullptr)
	, address(nullptr)
	, elementAddress(nullptr)
	, handleContextMap(nullptr)
	, walkComposite(WalkNull)
	, walkCompositeField(WalkNull)
	, walkCompositeFields(WalkNull)
	, walkPrimitive(WalkNull)
	, walkEnum(WalkNull)
	, walkArray(WalkNull)
{
}

//-----------------------------------//

void ReflectionSetHandleContext( ReflectionHandleContextMap* handleContextMap,
								    ReflectionHandleContext context)
{
	assert(handleContextMap && "Expected a valid context map");
	if (!handleContextMap ) return;

	(*handleContextMap)[context.type] = context;
}

//-----------------------------------//

bool ReflectionFindHandleContext( ReflectionHandleContextMap* handleContextMap,
								    Class* klass, ReflectionHandleContext& ctx)
{
	assert(handleContextMap && "Expected a valid context map");
	if (!handleContextMap ) return false;

	auto it = handleContextMap->find(klass);
	
	if( it != handleContextMap->end() )
	{
		ctx = it->second;
		return true;
	}
	else if( ClassHasParent(klass) )
	{
		Class* parent = ClassGetParent(klass);
		return ReflectionFindHandleContext(handleContextMap, parent, ctx);
	}

	return false;
}

//-----------------------------------//

Serializer::Serializer()
	: alloc(nullptr)
	, stream(nullptr)
	, object(nullptr)
	, load(nullptr)
	, save(nullptr)
{
	serializeContext.loading = false;
	deserializeContext.loading = true;
}

//-----------------------------------//

Serializer::~Serializer()
{
}

//-----------------------------------//

void SerializerDestroy(Serializer* serializer)
{
	Deallocate(serializer);
}

//-----------------------------------//

static void ReflectionWalkPrimitive(ReflectionContext* context)
{
	if( !context->walkPrimitive ) return;

	Primitive* type = context->primitive;
	void* address = context->address;
	ValueContext& vc = context->valueContext;
	
	switch(type->kind)
	{
	case PrimitiveTypeKind::Bool:
		vc.b = *(bool*) address;
		break;
	case PrimitiveTypeKind::Int8:
		vc.i8 = *(sint8*) address;
		break;
	case PrimitiveTypeKind::Uint8:
		vc.u8 = *(uint8*) address;
		break;
	case PrimitiveTypeKind::Int16:
		vc.i16 = *(int16*) address;
		break;
	case PrimitiveTypeKind::Uint16:
		vc.u16= *(uint16*) address;
		break;
	case PrimitiveTypeKind::Int32:
		vc.i32 = *(int32*) address;
		break;
	case PrimitiveTypeKind::Uint32:
		vc.u32 = *(uint32*) address;
		break;
	case PrimitiveTypeKind::Int64:
		vc.i64 = *(int64*) address;
		break;
	case PrimitiveTypeKind::Uint64:
		vc.u64 = *(uint64*) address;
		break;
	case PrimitiveTypeKind::Float:
		vc.f32 = *(float*) address;
		break;
	case PrimitiveTypeKind::Color:
		vc.c = *(Color*) address;
		break;
	case PrimitiveTypeKind::Vector3:
		vc.v = *(Vector3*) address;
		break;
	case PrimitiveTypeKind::Quaternion:
		vc.q = *(Quaternion*) address;
		break;
	case PrimitiveTypeKind::String:
		vc.s = (String*) address;
		break;
	default:
		assert( false );
	}

	context->walkPrimitive(context, ReflectionWalkType::Element);
}

//-----------------------------------//

static void ReflectionWalkEnum(ReflectionContext* context)
{
	if( !context->walkEnum ) return;

	ValueContext& vc = context->valueContext;
	vc.i32 = *(int32*) ClassGetFieldAddress(context->object, context->field);

	context->walkEnum(context, ReflectionWalkType::Element);
}

//-----------------------------------//

ReferenceCounted* NullResolve(HandleId) { return 0; }
void NullDestroy(HandleId) {}

static void ReflectionWalkType(ReflectionContext* context, Type* type);

static bool ReflectionWalkPointer(ReflectionContext* context)
{
	void* address = context->elementAddress;
	const Field* field = context->field;

#ifdef ENABLE_MEMORY_SHARED_PTR
	if(FieldIsSharedPointer(field))
	{
		std::shared_ptr<Object>* shared = (std::shared_ptr<Object>*) address;
		address = shared->get();
	}
	else
#endif

	if(FieldIsRefPointer(field))
	{
		RefPtr<Object>* ref = (RefPtr<Object>*) address;
		address = ref->get();
	}
	else if(FieldIsRawPointer(field))
	{
		address = context->elementAddress;
		address = *(Object**) address;
	}
	else if(FieldIsHandle(field))
	{
		typedef Handle<Object, NullResolve, NullDestroy> ObjectHandle;
		ObjectHandle* handle = (ObjectHandle*) address;
		HandleId id = handle->getId();
		
		Class* klass = (Class*) field->type;

		ReflectionHandleContext hc;
		if( !ReflectionFindHandleContext(context->handleContextMap, klass, hc) )
		{
			LogDebug("No handle context found for class '%s'", klass->name);
			return false;
		}

		address = (Object*) HandleFind(hc.handles, id);
		if( !address ) return false;
	}

	context->elementAddress = address;
	context->object = (Object*) context->elementAddress;

	return true;
}

//-----------------------------------//

static void ReflectionWalkArray(ReflectionContext* context)
{
	const Field* field = context->field;
	std::vector<byte>& array = *(std::vector<byte>*) context->address;

	uint16 elementSize = ReflectionArrayGetElementSize(context->field);
	uint32 arraySize = array.size() / elementSize;

	context->arraySize = arraySize;
	context->walkArray(context, ReflectionWalkType::Begin);
		
	for(size_t i = 0; i < arraySize; i++)
	{
		void* address = (&array[0] + i * elementSize);
		
		context->elementAddress = address;
		
		if(FieldIsPointer(field) && !ReflectionWalkPointer(context))
			continue;

		Object* object = context->object;
		Type* type = context->type;

		context->object = (Object*) context->elementAddress;
		context->type = field->type;

		bool isObject = ClassInherits((Class*) context->type, ObjectGetType());
		if( isObject ) context->type = ClassGetType(context->object);

		if( isObject && !context->type )
		{
			LogDebug("Could not get type out of object");
			return;
		}

		context->walkArray(context, ReflectionWalkType::ElementBegin);
		ReflectionWalkType(context, context->type);
		context->walkArray(context, ReflectionWalkType::ElementEnd);

		context->object = object;
		context->type = type;
	}

	context->walkArray(context, ReflectionWalkType::End);
}

//-----------------------------------//

static ReflectionWalkFunction HandleFindSerializeFunction(ReflectionContext* context, Class* klass)
{
	assert(context && "Expected a valid reflection context");
	if( !klass ) return nullptr;

	ReflectionHandleContext handleContext;
	if (!ReflectionFindHandleContext(context->handleContextMap, klass, handleContext))
		return nullptr;

	if( handleContext.serialize )
		return handleContext.serialize;
	
	Class* parent = klass->parent;
	return HandleFindSerializeFunction(context, parent);
}

//-----------------------------------//

void ReflectionWalkCompositeField(ReflectionContext* context)
{
	const Field* field = context->field;

	if( !FieldIsSerializable(field) )
	{
		LogDebug("Ignoring field '%s'", field->name);
		return;
	}

	void* address = context->address;
	void* elementAddress = context->elementAddress;
	Type* type = context->type;
	Object* object = context->object;

 	context->address = ClassGetFieldAddress(context->object, field);
	context->elementAddress = context->address;
	context->type = field->type;

	if( ReflectionIsComposite(field->type) )
		context->object = (Object*) context->address;

	if( FieldIsPointer(field) && !ReflectionWalkPointer(context) )
		goto exit;

	context->walkCompositeField(context, ReflectionWalkType::Begin);

	ReflectionWalkFunction handleSerialize = nullptr;
	
	if (ReflectionIsComposite(field->type))
		HandleFindSerializeFunction(context, (Class*) field->type);

	// Check for custom handle serialize functions.
	if( FieldIsHandle(field) && handleSerialize )
	{
		handleSerialize(context, ReflectionWalkType::Element);
	}
	else if( field->serialize )
	{
		field->serialize(context, ReflectionWalkType::Element);
	}
	else if( FieldIsArray(field) )
	{
		ReflectionWalkArray(context);
	}
	else if( FieldIsPointer(field) )
	{
		Object* object = context->object;
		Type* type = context->type;

		context->type = ClassGetType(context->object);
		ReflectionWalkType(context, context->type);

		context->type = type;
		context->object = object;
	}
	else
	{
		ReflectionWalkType(context, field->type);
	}

	context->walkCompositeField(context, ReflectionWalkType::End);

exit:

	context->address = address;
	context->elementAddress = elementAddress;
	context->type = type;
	context->object = object;
}

//-----------------------------------//

void ReflectionWalkComposite(ReflectionContext* context)
{
	bool isTopComposite = context->composite == context->objectClass;

	if( isTopComposite )
		context->walkComposite(context, ReflectionWalkType::Begin);

	if( ClassHasParent(context->composite) )
	{
		Class* current = context->composite;
		
		context->composite = ClassGetParent(current);
		ReflectionWalkComposite(context);
		
		context->composite = current;
	}

	const std::vector<Field*>& fields = context->composite->fields;

	const Field* field = context->field; 

	for( size_t i = 0; i < fields.size(); i++ )
	{
		context->field = fields[i];
		ReflectionWalkCompositeField(context);
	}

	context->field = field;

	if( isTopComposite )
		context->walkComposite(context, ReflectionWalkType::End);
}

//-----------------------------------//

static void ReflectionWalkType(ReflectionContext* context, Type* type)
{
	bool hasCustomSerialize = type->serialize != 0;

	switch(type->kind)
	{
	case TypeKind::Composite:
	{
		Class* objectClass = context->objectClass;
		Class* composite = context->composite;

		context->objectClass = (Class*) type;
		context->composite = context->objectClass;

		// If the type has a custom serializer, call it.
		if( hasCustomSerialize )
			type->serialize(context, ReflectionWalkType::Element);
		else
			ReflectionWalkComposite(context);

		context->objectClass = objectClass;
		context->composite = composite;
		break;
	}
	case TypeKind::Primitive:
		context->primitive = (Primitive*) type;
		ReflectionWalkPrimitive(context);
		break;
	case TypeKind::Enumeration:
		context->enume = (Enum*) type;
		ReflectionWalkEnum(context);
		break;
	}
}

//-----------------------------------//

void ReflectionWalk(const Object* object, ReflectionContext* context)
{
	if( !context ) return;

	Class* objectClass = ClassGetType(object);
	if( !objectClass ) return;

	context->object = (Object*) object;
	context->objectClass = objectClass;
	context->composite = objectClass;
	
	ReflectionWalkType(context, objectClass);
}

//-----------------------------------//

Object* SerializerLoad(Serializer* serializer)
{
	if( !serializer->load ) return nullptr;
	if( !serializer->stream ) return nullptr;

	Object* object = serializer->load(serializer);
	StreamClose(serializer->stream);

	return object;
}

//-----------------------------------//

bool SerializerSave(Serializer* serializer, const Object* object)
{
	if( !serializer->save ) return false;
	if( !serializer->stream ) return false;
	
	serializer->save(serializer, object);
	StreamClose(serializer->stream);

	serializer->object = nullptr;

	return true;
}

//-----------------------------------//

Object* SerializerLoadObjectFromFile(Serializer* serializer, const Path& file)
{
	Stream* stream = StreamCreateFromFile(serializer->alloc, file.c_str(), StreamMode::Read);	
	serializer->stream = stream;

	Object* object = SerializerLoad(serializer);
	StreamDestroy(stream);

	return object;
}

//-----------------------------------//

bool SerializerSaveObjectToFile(Serializer* serializer, const Path& file, Object* object)
{
	Stream* stream = StreamCreateFromFile(serializer->alloc, file.c_str(), StreamMode::Write);
	if( !stream ) return false;

	serializer->stream = stream;
	serializer->object = object;
	
	if( !SerializerSave(serializer, object) )
		return false;

	StreamDestroy(stream);

	return true;
}

//-----------------------------------//

typedef std::vector<RefPtr<ReferenceCounted>> ObjectRefPtrArray;
typedef std::vector<Object*> ObjectRawPtrArray;

void* ReflectionArrayResize( ReflectionContext* context, void* address, uint32 size )
{
	const Field* field = context->field;

	if( FieldIsRawPointer(field) )
	{
		ObjectRawPtrArray* array = (ObjectRawPtrArray*) address;
		array->resize(size);
		return &array->front();
	}
	else if( FieldIsRefPointer(field) )
	{
		ObjectRefPtrArray* array = (ObjectRefPtrArray*) address;
		array->resize(size);
		return &array->front();
	}
#if 0
	else if( FieldIsSharedPointer(field) )
	{
		ObjectSharedPtrArray* array = (ObjectSharedPtrArray*) address;
		array->resize(size);
		return &array->front();
	}
#endif
	else if( field->resize )
	{
		return field->resize(address, size);
	}
	
	LogAssert("Unknown array type");
	return nullptr;
}

//-----------------------------------//

uint16 ReflectionArrayGetElementSize(const Field* field)
{
	if( FieldIsPointer(field) )
		return field->pointer_size;
	else
		return field->size;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif