/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Handle.h"
#include "Core/Reflection.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Math/Quaternion.h"
#include "Core/String.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

union API_CORE ValueContext
{
	bool   b;
	sint8  i8;
	uint8  u8;
	sint16 i16;
	uint16 u16;
	sint32 i32;
	uint32 u32;
	sint64 i64;
	uint64 u64;
	float  f32;
	Vector3P v;
	ColorP c;
	QuaternionP q;
	String* s;
	const char* cs;
};

enum struct ReflectionWalkType : uint8
{
	Begin,
	End,
	Element,
	ElementBegin,
	ElementEnd,
};

//-----------------------------------//

struct Type;
struct Enum;
struct Class;
struct Field;
struct Primitive;
struct Object;
struct ReflectionContext;

// Walks the object calling the given reflection context.
API_CORE void ReflectionWalk(const Object*, ReflectionContext*);

// Walks the composite object.
API_CORE void ReflectionWalkComposite(ReflectionContext*);

// Walks the composite object field.
API_CORE void ReflectionWalkCompositeField(ReflectionContext*);

//-----------------------------------//

/**
 * Handles need to be resolved when they are walked via reflection.
 * The client code is responsible for setting up the mapping functions
 * that allow the reflection walking code to obtain handle details.
 */

typedef HandleId (*ReflectionDeserializeHandleFn)(const char*); 

struct API_CORE ReflectionHandleContext
{
	Class* type;
	HandleManager* handles;
	ReflectionWalkFunction serialize;
	ReflectionDeserializeHandleFn deserialize;
};

typedef Hash<ReflectionHandleContext> ReflectionHandleContextMap;

API_CORE void ReflectionSetHandleContext(
	ReflectionHandleContextMap*, ReflectionHandleContext context);

API_CORE bool ReflectionFindHandleContext(
	ReflectionHandleContextMap*, Class* klass, ReflectionHandleContext& ctx);

//-----------------------------------//

struct API_CORE ReflectionContext
{
	ReflectionContext();

	bool loading;
	void* userData;
	
	Object* object;
	Class* objectClass;

	Type* type;
	Primitive* primitive;
	Class* composite;
	Enum*  enume;

	ValueContext valueContext;
	ReflectionHandleContextMap* handleContextMap;

	const Field* field;
	void* address;
	void* elementAddress;
	uint32 arraySize;

	ReflectionWalkFunction walkComposite;
	ReflectionWalkFunction walkCompositeField;
	ReflectionWalkFunction walkCompositeFields;
	ReflectionWalkFunction walkPrimitive;
	ReflectionWalkFunction walkEnum;
	ReflectionWalkFunction walkArray;
};

//-----------------------------------//

struct Stream;
struct Serializer;

typedef Object* (*SerializerLoadFunction)(Serializer*);
typedef bool    (*SerializerSaveFunction)(Serializer*, const Object*);

struct API_CORE Serializer
{
	Serializer();
	virtual ~Serializer();

	Allocator* alloc;
	Stream* stream;
	Object* object;

	ReflectionContext serializeContext;
	ReflectionContext deserializeContext;

	SerializerLoadFunction load;
	SerializerSaveFunction save;
};

//-----------------------------------//

// Creates a new JSON serializer.
API_CORE Serializer* SerializerCreateJSON(Allocator*, ReflectionHandleContextMap*);

// Creates a new binary serializer.
API_CORE Serializer* SerializerCreateBinary(Allocator*, ReflectionHandleContextMap*);

// Destroys the serializer.
API_CORE void SerializerDestroy(Serializer*);

// Loads an object from a stream.
API_CORE Object* SerializerLoad(Serializer*);
API_CORE bool SerializerSave(Serializer*, const Object* object);

// Wrappers for file-based loading and saving.
API_CORE Object* SerializerLoadObjectFromFile(Serializer*, const Path&);
API_CORE bool SerializerSaveObjectToFile(Serializer*, const Path&, Object* object);

//-----------------------------------//

NAMESPACE_CORE_END