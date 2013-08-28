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
	int8   i8;
	uint8  u8;
	int16  i16;
	uint16 u16;
	int32  i32;
	uint32 u32;
	int64  i64;
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

typedef std::map<Class*, ReflectionHandleContext> ReflectionHandleContextMap;

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

class Stream;
class Serializer;

enum SerializerType
{
	Json,
	Binary
};

class API_CORE Serializer
{
public
	:
	/**
	 * Create new serializer
	 * @param allocator allocator used for this instance 
	 */
	Serializer(Allocator* allocator);
	virtual ~Serializer();

	/**
	 * Loads an object from stream.
	 */
	virtual Object* load() = 0;

	/**
	 * Saves an object to stream.
	 * @param obj object to save
	 */
	virtual bool save(const Object* obj) = 0;

	/**
	 * Wrapper for file-based loading.
	 * @param serializer serializer to perform the loading
	 * @param file file path to create the stream from
	 */
	static Object* loadObjectFromFile(Serializer& serializer, 
		const Path& file);
	
	/**
	 * Wrapper for file-based saving.
	 * @param serializer serializer to perform the saving
	 * @param file file path to create the stream from
	 * @param object object to save
	 */
	static bool saveObjectToFile(Serializer& serializer, 
		const Path& file, Object* object);

	Allocator* allocator; //!< allocator used for the serializer instance
	Stream* stream; //!< stream to save and load objects from
	Object* object; //!< object being loaded or saved

	ReflectionContext serializeContext; //!< reflection context used for serialization
	ReflectionContext deserializeContext; //!< reflection context used for deserialization
};

//-----------------------------------//

NAMESPACE_CORE_END