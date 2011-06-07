/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_BEGIN

//-----------------------------------//

struct Vector3;
class Color;
class Quaternion;

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

struct Object;
struct ReflectionContext;

// Walks the object calling the given reflection context.
void ReflectionWalk(Object*, ReflectionContext*);

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

// Creates a new JSON serializer.
Serializer* SerializerCreateJSON(Allocator*);

// Creates a new binary serializer.
Serializer* SerializerCreateBinary(Allocator*);

// Destroys the serializer.
void SerializerDestroy(Serializer*);

// Loads an object from a stream.
Object* SerializerLoad(Serializer*);
void SerializerSave(Serializer*);

//-----------------------------------//

NAMESPACE_END