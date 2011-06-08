/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

/**
 * Provides types with a fast RTTI (Runtime Type Information) system that 
 * will be used for fast dynamic type checking, reflection and serialization.
 */

struct API_CORE Type
{
	enum_class MetaType : uint8
	{
		Primitive,
		Composite,
		Enumeration,
	};

	Type() {}

	Type(MetaType type, const char* name, uint16 size)
		: type(type), name(name), size(size) {}

	// Meta type of the type.
	MetaType type;

	// Name of the type.
	const char* name;

	// Size of the type.
	uint16 size;
};

// Gets if this type represents a primitive type.
API_CORE bool ReflectionIsPrimitive(const Type*);

// Gets if this type represents a composite type.
API_CORE bool ReflectionIsComposite(const Type*);

// Gets if this type represents an enum type.
API_CORE bool ReflectionIsEnum(const Type*);

// Returns if this type is of the given type.
API_CORE bool ReflectionIsEqual(const Type*, const Type*);

//-----------------------------------//

typedef API_CORE std::map<const char*, Type*, RawStringCompare> ReflectionTypeMap;

// Gets the type for a given type.
#define ReflectionGetType(T) T##GetType()

// Gets a type given a name.
API_CORE Type* ReflectionFindType(const char*);

// Registers a new type.
API_CORE void ReflectionRegisterType(Type*);

// Gets the type registry.
API_CORE ReflectionTypeMap& ReflectionGetTypeMap();

//-----------------------------------//

typedef std::map<const char*, int32, RawStringCompare> EnumValuesMap;
typedef std::pair<const char*, int32> EnumValuesPair;

struct API_CORE Enum : public Type
{
	EnumValuesMap values;
};

// Adds a new enumeration to this enum.
API_CORE void EnumAddValue(Enum*, const char* name, int32 value);

// Gets the value of this enum name.
API_CORE int32 EnumGetValue(Enum*, const char* name);

// Gets the name of this enum value.
API_CORE const char* EnumGetValueName(Enum*, int32 value);

//-----------------------------------//

struct Field;
struct Object;

typedef void* (*ClassCreateFunction)(Allocator*);

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking and reflection.
 * Main use is for serialization and property introspection.
 */

struct API_CORE Class : public Type
{
	Class() : parent(nullptr), create_fn(nullptr) { }

	// Parent of the type.
	Class* parent;

	// Factory function.
	ClassCreateFunction create_fn;

	// Keeps track of the type fields.
	std::vector<Field*> fields;

	// Keeps track of the childs of the class.
	std::vector<Class*> childs;
};

// Returns the parent of the class.
#define ClassGetParent(klass) (klass->parent)

// Returns if this class has a parent.
#define ClassHasParent(klass) (ClassGetParent(klass) != nullptr)

// Returns if this type inherits from the given type.
API_CORE bool ClassInherits(const Class* klass, const Class* test);

// Returns the type from the given class instance at runtime.
API_CORE Class* ClassGetType(const Object*);

// Returns if the class is abstract.
API_CORE bool ClassIsAbstract(const Class*);

// Gets the field with the given name.
API_CORE Field* ClassGetField(const Class*, const char* name);

// Gets the field with the given name.
API_CORE void* ClassGetFieldAddress(const void*, const Field*);

// Creates a new instance of the class.
API_CORE void* ClassCreateInstance(const Class*, Allocator*);

//-----------------------------------//

typedef void (*FieldSetterFunction)(void* object, void* value);

struct FieldQualifier
{
	enum Enum
	{
		Array			= 1 << 0,
		Map				= 1 << 1,
		Set				= 1 << 2,
		Handle			= 1 << 3,
		RawPointer		= 1 << 4,
		SharedPointer	= 1 << 5,
		RefPointer		= 1 << 6,
		ReadOnly		= 1 << 7,
	};
};

struct API_CORE Field
{
	Type* type;
	const char* name;
	uint8 qualifiers;
	uint16 offset;
	uint16 size;
	uint16 pointer_size;
	FieldSetterFunction setter_fn;
	std::vector<const char*> aliases;
};

// Returns if the field has the given qualifier.
API_CORE bool FieldHasQualifier(const Field*, FieldQualifier::Enum);

// Sets the given qualifier in the field.
API_CORE void FieldSetQualifier(Field*, FieldQualifier::Enum);

// Sets the given setter in the field.
API_CORE void FieldSetSetter(Field*, FieldSetterFunction = nullptr);

#define FieldIsArray(f)         FieldHasQualifier(f, FieldQualifier::Array)
#define FieldIsHandle(f)        FieldHasQualifier(f, FieldQualifier::Handle)
#define FieldIsRawPointer(f)    FieldHasQualifier(f, FieldQualifier::RawPointer)
#define FieldIsRefPointer(f)    FieldHasQualifier(f, FieldQualifier::RefPointer)
#define FieldIsSharedPointer(f) FieldHasQualifier(f, FieldQualifier::SharedPointer)
#define FieldIsReadOnly(f) FieldHasQualifier(f, FieldQualifier::ReadOnly)
#define FieldIsPointer(f)       (FieldIsRawPointer(f) || FieldIsRefPointer(f) || FieldIsSharedPointer(f) || FieldIsHandle(f))

//-----------------------------------//

struct API_CORE Primitive : public Type
{
	enum_class PrimitiveType
	{
		Bool,
		Int32,
		Uint32,
		Float,
		String,
		Bitfield,
		Color,
		Vector3,
		Quaternion,
	};

	Primitive() {}

	Primitive(PrimitiveType type, const char* name, uint16 size)
		: Type(Type::Primitive, name, size), type(type) {}

	PrimitiveType type;

    static Primitive s_bool;
    static Primitive s_int32;
    static Primitive s_uint32;
    static Primitive s_float;
    static Primitive s_string;
    static Primitive s_Vector3;
    static Primitive s_Color;
    static Primitive s_Quaternion;
    static Primitive s_Bitfield;
};

//-----------------------------------//

EXTERN_END

// Gets the value of the field in the object.
template<typename T>
const T& FieldGet( const Field* field, void* object )
{
	T* addr = (T*) ClassGetFieldAddress(object, field);
	return *addr;
}

// Sets the value of the field in the object.
template<typename T>
void FieldSet( Field* field, void* object, const T& value )
{
	T* addr = (T*) ClassGetFieldAddress(object, field);
	FieldSetterFunction setter = field->setter_fn;
	
	if(setter) setter(object, (void*) &value);
	else *addr = value;
}

//-----------------------------------//

NAMESPACE_END

#include "Core/Helpers.h"