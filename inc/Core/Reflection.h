/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/String.h"
#include <map>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Types can have metadata attached to them in the form of attributes.
 */
 enum class TypeAttributeKind : uint8
{
	String,
	Integer,
	KeyValue,
	Flags
};

//-----------------------------------//

/**
 * Provides types with a fast RTTI (Runtime Type Information) system that 
 * will be used for fast dynamic type checking, reflection and serialization.
 */
enum class TypeKind : uint8
{
	Primitive,
	Composite,
	Enumeration,
};

struct ReflectionContext;
enum struct ReflectionWalkType : uint8;

typedef void (*ReflectionWalkFunction)(ReflectionContext*, ReflectionWalkType);

class API_CORE Type
{
public:

	Type();

	/**
	 * Creates new type.
	 * @param kind type kind
	 * @param name name of the type
	 * @param size size of the type
	 */
	Type(TypeKind kind, const char* name, uint16 size);

	virtual ~Type();

	/**
	 * Checks if this type represents a primitive type.
	 * @returns whether type is primitive
	 */
	bool isPrimitive();

	/**
	 * Checks if this type represents a composite type.
	 * @returns whether type is composite
	 */
	bool isComposite();

	/**
	 * Checks if this type represents an enum type.
	 * @returns whether type is enum
	 */
	bool isEnum();

	/**
	 * Checks if this type is of the given type.
	 * @param type type to compare to
	 * @returns whether types are the same
	 */
	bool isEqual(const Type* type) const;

	/**
	 * Checks if this type is of the given type.
	 * @param type type to compare to
	 * @returns whether types are the same
	 */
	bool operator==(const Type& type) const;

	TypeKind kind; //!< Meta type of the type.

	const char* name; //!< Name of the type.

	uint16 size; //!< Size of the type.

	Vector<TypeAttribute> attributes;//!< Attributes of the type.

	ReflectionWalkFunction serialize; //!< Custom walk function.

};

typedef std::map<const char*, Type*, RawStringCompare> TypeMap;

//-----------------------------------//

/**
 * Keeps a map with the types in the database.
 */
class API_CORE ReflectionDatabase
{
public:

	/**
	 * Registers a new type in the reflection database.
	 * @param type type to register
	 * @returns whether registering was sucessful
	 */
	bool registerType(Type* type);

	/**
	 * Gets the reflection database.
	 * @returns static reflection database
	 */
	static ReflectionDatabase& GetDatabase();
	
	/**
	 * Gets a type given a name.
	 * @param name name of type to find
	 * @returns type pointer or nullptr if it doesn't find anything
	 */
	Type* findType(const char* name);

	TypeMap types; //!< map of type name to type
};

//-----------------------------------//

struct Object;
class Class;
class Field;

typedef uint16 ClassId;
typedef uint8 FieldId;

typedef void* (*ClassCreateFunction)(Allocator*);

typedef std::map<FieldId, Field*> ClassFieldIdMap;
typedef std::map<ClassId, Class*> ClassIdMap;

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking and reflection.
 * Main use is for serialization and property introspection.
 */
class API_CORE Class : public Type
{

public:

	Class();

	/**
	 * Returns whether this type inherits from the given type.
	 * @param test type to check if this class inherits from
	 * @returns whether class inherits from test
	 */
	bool inherits(const Class* test);

	/**
	 * Returns the type from the given class instance at runtime.
	 * @param object object to get type of
	 * @returns pointer to class type or nullptr if object not set
	 */
	static Class* GetType(const Object* object);

	/**
	 * Checks if the class is abstract.
	 * @returns whether class is abstract
	 */
	bool isAbstract();

	/**
	 * Adds a new field to the given class.
	 * @param field field to add
	 */
	void addField(Field* field);

	/**
	 * Gets the field with the given name.
	 * @param name name of the field to get
	 * @returns pointer to field or nullptr if name not found
	 */
	Field* getField(const char* name);

	/**
	 * Gets a field from a class hierarchy by its id.
	 * @param id id of the field to get
	 * @returns field pointer or nullptr if id not found
	 */
	Field* getFieldById(FieldId id);

	/**
	 * Gets a pointer to a given field.
	 * @param object object that contains the field 
	 * @param field field that you want the address of
	 * @returns address or nullptr if either object or field are not set
	 */
	static void* GetFieldAddress(const void* object, const Field* field);

	/**
	 * Creates a new instance of the class.
	 * @param allocator allocator where you want to instantiate the class
	 * @returns pointer to instance or nullptr if class is abstract
	 */
	void* createInstance(Allocator* allocator);

	/**
	 * Calculates the class id of this class.
	 * @returns class id of this class
	 */
	ClassId calculateId();

	/**
	 * Gets the class id map.
	 * @returns static class id map
	 */
	static ClassIdMap& GetIdMap();

	/**
	 * Register class in the class id map.
	 */
	void registerClass();

	/**
	 * Get this class's parent.
	 * @returns pointer to the parent of this class.
	 */
	Class * getParent();

	/**
	 * Check if class has a parent.
	 * @returns whether this class has a parent
	 */
	bool hasParent();

	/**
	 * Gets the class by id.
	 * @param id id of the class to retrieve
	 * @returns pointer to class or nullptr if id not found
	 */
	Class* getById(ClassId id);

	ClassId id; //!< Class id.

	Class* parent; //!< Parent of the type.

	ClassCreateFunction create_fn; //!< Factory function.

	Vector<Field*> fields; //!< Keeps track of the type fields.

	ClassFieldIdMap fieldIds; //!< Keeps track of the type fields by id.

	Vector<Class*> childs; //!< Keeps track of the childs of the class.

};


//-----------------------------------//

typedef void (*FieldSetterFunction)(void* object, void* value);
typedef void* (*FieldResizeFunction)(void* object, size_t size);

enum struct FieldQualifier : uint16
{
	Array           = 1 << 0,
	Map             = 1 << 1,
	Set             = 1 << 2,
	Handle          = 1 << 3,
	RawPointer      = 1 << 4,
	SharedPointer   = 1 << 5,
	RefPointer      = 1 << 6,
	ReadOnly        = 1 << 7,
	NoSerialize     = 1 << 8
};

class API_CORE Field
{
public:

	Field();

	/**
	 * Checks if the field has the given qualifier.
	 * @param qual qualifier to check
	 * @returns whether field has qualifier
	 */
	bool hasQualifier(FieldQualifier qual) const;

	/**
	 * Sets the given qualifier in the field.
	 * @param qual qualifier to set
	 */
	void setQualifier(FieldQualifier qual);

	/**
	 * Sets the given setter in the field.
	 * @param fn setter function
	 */
	void setSetter(FieldSetterFunction fn);

	Type* type; //!< field type
	FieldId id; //!< field id
	const char* name; //!< field name
	std::vector<const char*> aliases; //!< field aliases
	uint16 size; //!< field size
	uint16 offset; //!< field offset
	uint16 pointer_size; //!< field pointer size 
	FieldQualifier qualifiers; //!< field qualifiers
	FieldSetterFunction setter; //!< field setter function
	FieldResizeFunction resize; //!< field resize function
	ReflectionWalkFunction serialize; //!< field serialize function

	static const FieldId FieldInvalid = 127; //invalid field id
};

#define FieldIsArray(f)         f->hasQualifier(FieldQualifier::Array)
#define FieldIsHandle(f)        f->hasQualifier(FieldQualifier::Handle)
#define FieldIsRawPointer(f)    f->hasQualifier(FieldQualifier::RawPointer)
#define FieldIsRefPointer(f)    f->hasQualifier(FieldQualifier::RefPointer)
#define FieldIsSharedPointer(f) f->hasQualifier(FieldQualifier::SharedPointer)
#define FieldIsReadOnly(f)      f->hasQualifier(FieldQualifier::ReadOnly)
#define FieldIsSerializable(f)  (!f->hasQualifier(FieldQualifier::NoSerialize))
#define FieldIsPointer(f)       (FieldIsRawPointer(f) || FieldIsRefPointer(f) || FieldIsSharedPointer(f) || FieldIsHandle(f))

//-----------------------------------//

enum struct PrimitiveTypeKind : uint8
{
	Bool,
	Int8,
	Uint8,
	Int16,
	Uint16,
	Int32,
	Uint32,
	Int64,
	Uint64,
	Float,
	String,
	Color,
	Vector3,
	Quaternion,
};

class API_CORE Primitive : public Type
{
public: 

	Primitive();

	/**
	 * Create a primitive.
	 * @param kind primitive kind
	 * @param name primitive name
	 * @param size primitive size
	 */
	Primitive(PrimitiveTypeKind kind, const char* name, uint16 size);

	PrimitiveTypeKind kind; //!< primitive kind
};

class API_CORE PrimitiveBuiltins
{
public: 

	PrimitiveBuiltins();

	/**
	 * Gets the primitive builtin types.
	 * @returns static builtin primitives 
	 */
	static PrimitiveBuiltins& GetBuiltins();

	Primitive p_bool; //!< bool primitive
	Primitive p_int8; //!< char primitive
	Primitive p_uint8; //!< unsigned char primitive
	Primitive p_int16; //!< short primitive
	Primitive p_uint16; //!< unsigned short primitive
	Primitive p_int32; //!< int primitive
	Primitive p_uint32; //!< unsigned int primitive
	Primitive p_int64; //!< long long primitive
	Primitive p_uint64; //!< unsigned long long primitive
	Primitive p_float; //!< float primitive
	Primitive p_double; //!< double primitive
	Primitive p_string; //!< string primitive
	Primitive p_Vector3; //!< vector3 primitive
	Primitive p_Color; //!< color primitive
	Primitive p_Quaternion; //!< quaternion primitive
};


//-----------------------------------//

typedef std::map<const char*, int32, RawStringCompare> EnumValuesMap;
typedef std::pair<const char*, int32> EnumValuesPair;

class API_CORE Enum : public Type
{
public:

	/**
	 * Adds a new enumeration to this enum.
	 * @param name enum name
	 * @param value enum value
	 */
	void addValue(const char* name, int32 value);

	/**
	 * Gets the value of this enum name.
	 * @param name enum name
	 * @returns enum value or -1 if not found
	 */
	int32 getValue(const char* name);

	/**
	 * Gets the name of this enum value.
	 * @param value enum value
	 * @returns enum name or nullptr if not found
	 */
	const char* getValueName(int32 value);

	EnumValuesMap values; //!< map enum names to enum values
};

//-----------------------------------//

/**
 * Gets the value of the field in the object.
 * @param field field to obtain value from
 * @param object object that contains field
 * @returns value
 */
template<typename T>
const T& FieldGet(const Field* field, void* object )
{
	T* addr = (T*) Class::GetFieldAddress(object, field);
	return *addr;
}

/**
 * Sets the value of the field in the object.
 * @param field field to set value in
 * @param object object that contains field
 * @param value value to set
 */
template<typename T>
void FieldSet( const Field* field, void* object, const T& value )
{
	T* addr = (T*) Class::GetFieldAddress(object, field);
	FieldSetterFunction setter = field->setter;
	
	if(setter) setter(object, (void*) &value);
	else *addr = value;
}

/**
 * Recursively finds and creates instances of child classes.
 * @param klass class to instance children from
 * @param alloc allocator for children
 * @param instances out vector with resulting children
 */
template<typename T>
void ClassCreateChilds(const Class* klass, Allocator* alloc, Vector<T*>& instances)
{
	for( size_t i = 0; i < klass->childs.size(); i++ )
	{
		Class* child = klass->childs[i];
		if( !child ) continue;

		ClassCreateChilds(child, alloc, instances);

		if( ClassIsAbstract(child) )
			continue;

		T* object = (T*) child->createInstance(alloc);
		if(!object) continue;

		instances.push_back(object);
	}
}

//-----------------------------------//

NAMESPACE_CORE_END

#include "Core/ReflectionHelpers.h"

#ifdef COMPILER_CLANG
#define fl_reflect(b) __attribute__((annotate("fl:reflect="#b)))
#define fl_attribute(...) __attribute__((annotate("fl:attributes="#__VA_ARGS__)))
#else
#define fl_reflect(b)
#define fl_attribute(...)
#endif