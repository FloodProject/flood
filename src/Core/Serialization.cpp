/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_SERIALIZATION

#include "Core/Serialization.h"
#include "Core/ReferenceCount.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Log.h"
#include "Core/Stream.h"

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/EulerAngles.h"
#include "Math/Color.h"

NAMESPACE_BEGIN

//-----------------------------------//

static std::map<Class*, ReflectionHandleContext> gs_ReflectionHandleMap;

void ReflectionSetHandleContext(ReflectionHandleContext context)
{
	gs_ReflectionHandleMap[context.type] = context;
}

//-----------------------------------//

bool ReflectionFindHandleContext(Class* klass, ReflectionHandleContext& ctx)
{
	auto it = gs_ReflectionHandleMap.find(klass);
	
	if( it != gs_ReflectionHandleMap.end() )
	{
		ctx = it->second;
		return true;
	}
	else if( ClassHasParent(klass) )
	{
		Class* parent = ClassGetParent(klass);
		return ReflectionFindHandleContext(parent, ctx);
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
{ }

//-----------------------------------//

Serializer::~Serializer()
{ }

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
	
	switch(type->type)
	{
	case Primitive::Bool:
		vc.b = (bool*) address;
		break;
	case Primitive::Int32:
		vc.i32 = (int32*) address;
		break;
	case Primitive::Uint32:
		vc.u32 = (uint32*) address;
		break;
	case Primitive::Float:
		vc.f = (float*) address;
		break;
	case Primitive::String:
		vc.s = (String*) address;
		break;
	case Primitive::Color:
		vc.c = (Color*) address;
		break;
	case Primitive::Vector3:
		vc.v = (Vector3*) address;
		break;
	case Primitive::Quaternion:
		vc.q = (Quaternion*) address;
		break;
	case Primitive::Bitfield:
		vc.bf = (int32*) address;
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
	vc.i32 = (int32*) ClassGetFieldAddress(context->object, context->field);

	context->walkEnum(context, ReflectionWalkType::Element);
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

static void ReflectionWalkType(ReflectionContext* context, Type* type);

static bool ReflectionWalkPointer(ReflectionContext* context)
{
	void* address = context->elementAddress;
	const Field* field = context->field;
	bool resolve = true;

	if(FieldIsSharedPointer(field))
	{
		std::shared_ptr<Object>* shared = (std::shared_ptr<Object>*) address;
		address = shared->get();
	}
	else if(FieldIsRefPointer(field))
	{
		RefPtr<Object>* ref = (RefPtr<Object>*) address;
		address = ref->get();
	}
	else if(FieldIsRawPointer(field))
	{
		address = context->elementAddress;
		address = *(Object**) address;
		
		resolve = false;
	}
	else if(FieldIsHandle(field))
	{
		typedef Handle<Object, 0, 0> ObjectHandle;
		ObjectHandle* handle = (ObjectHandle*) address;
		HandleId id = handle->id;
		
		Class* type = (Class*) field->type;

		ReflectionHandleContext hc;
		if( !ReflectionFindHandleContext(type, hc) )
		{
			LogDebug("No handle context found for class '%s'", type->name);
			return false;
		}

		address = HandleFind(hc.handles, id);
		if( !address ) return false;

		resolve = false;
	}

	assert( address != nullptr );
	context->elementAddress = address;

	if(resolve)
		context->object = *(Object**) context->elementAddress;
	else
		context->object = (Object*) context->elementAddress;

	return true;
}

//-----------------------------------//

static void ReflectionWalkArray(ReflectionContext* context)
{
	const Field* field = context->field;
	std::vector<byte>& array = *(std::vector<byte>*) context->address;

	uint16 elementSize = GetArrayElementSize(context->field);
	uint32 numElems = array.size() / elementSize;

	context->walkArray(context, ReflectionWalkType::Begin);
		
	for(size_t i = 0; i < numElems; i++)
	{
		void* address = (&array.front() + i * elementSize);
		
		context->elementAddress = address;
		
		if(FieldIsPointer(field) && !ReflectionWalkPointer(context))
			continue;

		Object* object = context->object;
		Type* type = context->type;

		context->object = (Object*) context->elementAddress;
		context->type = ClassGetType(context->object);

		context->walkArray(context, ReflectionWalkType::ElementBegin);
		ReflectionWalkType(context, context->type);
		context->walkArray(context, ReflectionWalkType::ElementEnd);

		context->object = object;
		context->type = type;
	}

	context->walkArray(context, ReflectionWalkType::End);
}

//-----------------------------------//

static void ReflectionWalkCompositeField(ReflectionContext* context)
{
	Field* field = context->field;

	if( !FieldIsSerializable(field) )
		return;
	
	void* address = context->address;
	void* elementAddress = context->elementAddress;
	Type* type = context->type;
	Object* object = context->object;

 	context->address = ClassGetFieldAddress(context->object, field);
	context->elementAddress = context->address;
	context->type = field->type;

	if( ReflectionIsComposite(field->type) )
		context->object = (Object*) context->address;

	context->walkCompositeField(context, ReflectionWalkType::Begin);

	if( FieldIsArray(field) )
	{
		ReflectionWalkArray(context);
	}
	else if( FieldIsPointer(field) )
	{
		if( !ReflectionWalkPointer(context) )
			goto exit;

		//Object* object = context->object;
		//Type* type = context->type;

		context->type = ClassGetType(context->object);
		//context->object = newObject;

		ReflectionWalkType(context, field->type);

		//context->type = type;
		//context->object = object;
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

static void ReflectionWalkComposite(ReflectionContext* context)
{
	bool isTopComposite = context->composite == context->klass;

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

	Field* field = context->field; 

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
	switch(type->type)
	{
	case Type::Composite:
	{
		Class* klass = context->klass;
		Class* composite = context->composite;

		context->klass = (Class*) type;
		context->composite = context->klass;

		ReflectionWalkComposite(context);

		context->klass = klass;
		context->composite = composite;
		break;
	}
	case Type::Primitive:
		context->primitive = (Primitive*) type;
		ReflectionWalkPrimitive(context);
		break;
	case Type::Enumeration:
		context->enume = (Enum*) type;
		ReflectionWalkEnum(context);
		break;
	}
}

//-----------------------------------//

void ReflectionWalk(Object* object, ReflectionContext* context)
{
	if( !context ) return;

	Class* klass = ClassGetType(object);
	if( !klass ) return;

	context->object = object;
	context->klass = klass;
	context->composite = klass;
	
	ReflectionWalkType(context, klass);
}

//-----------------------------------//

Object* SerializerLoad(Serializer* serializer)
{
	if( !serializer->load ) return nullptr;
	if( !serializer->stream ) return nullptr;

	serializer->load(serializer);
	StreamClose(serializer->stream);

	return serializer->object;
}

//-----------------------------------//

bool SerializerSave(Serializer* serializer)
{
	if( !serializer->save ) return false;
	if( !serializer->stream ) return false;
	
	serializer->save(serializer);
	StreamClose(serializer->stream);

	return true;
}

//-----------------------------------//

Object* SerializerLoadObjectFromFile(const Path& file)
{
	Serializer* json = SerializerCreateJSON( AllocatorGetHeap() );
	json->alloc = AllocatorGetHeap();

	Stream* stream = StreamCreateFromFile(AllocatorGetHeap(), file.c_str(), StreamMode::Read);	
	json->stream = stream;

	Object* object = SerializerLoad(json);

	SerializerDestroy(json);
	StreamDestroy(stream);

	return object;
}

//-----------------------------------//

bool SerializerSaveObjectToFile(const Path& file, Object* object)
{
	Serializer* json = SerializerCreateJSON( AllocatorGetHeap() );
	json->alloc = AllocatorGetHeap();

	Stream* stream = StreamCreateFromFile(AllocatorGetHeap(), file.c_str(), StreamMode::Write);
	if( !stream ) return false;

	json->stream = stream;
	json->object = object;
	
	if( !SerializerSave(json) )
		return false;

	SerializerDestroy(json);
	StreamDestroy(stream);

	return true;
}

//-----------------------------------//

NAMESPACE_END

#endif