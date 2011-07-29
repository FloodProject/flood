/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Handle.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Color;
struct Vector3;
struct Quaternion;

union API_CORE ValueContext
{
	bool* b;
	int32* i32;
	uint32* u32;
	float* f;
	int32* bf;
	String* s;
	Vector3* v;
	Color* c;
	Quaternion* q;
	const char* cs;
};

struct ReflectionWalkType
{
	enum Enum
	{
		Begin,
		End,
		Element,
		ElementBegin,
		ElementEnd,
	};
};

struct Class;
struct Object;
struct ReflectionContext;

// Walks the object calling the given reflection context.
API_CORE void ReflectionWalk(Object*, ReflectionContext*);

/* Handles need to be resolved when they are walked via reflection.
 * The client code is responsible for setting up the mapping functions
 * that allow the reflection walking code to obtain handle details.
 */

typedef HandleId (*ReflectionDeserializeHandleFn)(const char*); 

struct API_CORE ReflectionHandleContext
{
	Class* type;
	HandleManager* handles;
	ReflectionDeserializeHandleFn deserialize;
};

API_CORE void ReflectionSetHandleContext(ReflectionHandleContext context);
API_CORE bool ReflectionFindHandleContext(Class* klass, ReflectionHandleContext& ctx);

// Walking functions
typedef void (*ReflectionWalkFunc)(ReflectionContext*, ReflectionWalkType::Enum);
typedef void (*ReflectionWalkCompositeFunc)(ReflectionContext*, ReflectionWalkType::Enum);

struct Type;
struct Enum;
struct Class;
struct Field;
struct Primitive;

struct API_CORE ReflectionContext
{
	ReflectionContext() {}

	void* userData;

	Object* object;
	Class* klass;

	Type* type;
	Enum*  enume;
	Class* composite;
	Primitive* primitive;	
	
	ValueContext valueContext;

	Field* field;
	void* address;
	void* elementAddress;

	ReflectionWalkCompositeFunc walkComposite;
	ReflectionWalkFunc walkCompositeField;
	ReflectionWalkFunc walkPrimitive;
	ReflectionWalkFunc walkEnum;
	ReflectionWalkFunc walkArray;
};

//-----------------------------------//

struct Stream;
struct Serializer;

typedef void (*SerializerLoadFunc)(Serializer*);
typedef void (*SerializerSaveFunc)(Serializer*);

struct API_CORE Serializer
{
	Serializer();
	virtual ~Serializer();

	Allocator* alloc;

	Stream* stream;
	Object* object;

	ReflectionContext serializeContext;
	ReflectionContext deserializeContext;

	SerializerLoadFunc load;
	SerializerSaveFunc save;
};

#ifdef ENABLE_SERIALIZATION_JSON

// Creates a new JSON serializer.
API_CORE Serializer* SerializerCreateJSON(Allocator*);

#endif

#ifdef ENABLE_SERIALIZATION_BINARY

// Creates a new binary serializer.
API_CORE Serializer* SerializerCreateBinary(Allocator*);

#endif

// Destroys the serializer.
API_CORE void SerializerDestroy(Serializer*);

// Loads an object from a stream.
API_CORE Object* SerializerLoad(Serializer*);
API_CORE bool SerializerSave(Serializer*);

// Wrappers for file-based loading and saving.
API_CORE Object* SerializerLoadObjectFromFile(const Path&);
API_CORE bool SerializerSaveObjectToFile(const Path& file, Object* object);

//-----------------------------------//

NAMESPACE_CORE_END

