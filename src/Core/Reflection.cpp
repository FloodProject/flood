/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Log.h"
#include "Math/Hash.h"
#include "Math/Vector.h"
#include "Math/Color.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CLASS(Object)
REFLECT_CLASS_END()

//-----------------------------------//

bool ReflectionIsPrimitive(const Type* type)
{
	return type && type->type == Type::Primitive;
}

//-----------------------------------//

bool ReflectionIsComposite(const Type* type)
{
	return type && type->type == Type::Composite;
}

//-----------------------------------//

bool ReflectionIsEnum(const Type* type)
{
	return type && type->type == Type::Enumeration;
}

//-----------------------------------//

ReflectionDatabase& ReflectionGetDatabase()
{
	static ReflectionDatabase s_ReflectionTypes;
	return s_ReflectionTypes;
}

//-----------------------------------//

static void RegisterClass(Class* klass)
{
	uint32 hash = HashMurmur2(0xBEEF, (uint8*) klass->name, strlen(klass->name));
	klass->id = (ClassId) hash;

	//LogDebug("Registering class: '%s' with id '%u'", klass->name, klass->id);

	// Register as child class in the parent class.
	Class* parent = klass->parent;
	if( parent ) parent->childs.push_back(klass);

	// Register the class id in the map.
	ClassIdMap& ids = ClassGetIdMap();

	if( ids.find(klass->id) != ids.end() )
	{
		LogError("Class with the same id already exists: '%s'", klass->name);
		return;
	}

	ids[klass->id] = klass;
}

//-----------------------------------//

void ReflectionRegisterType(Type* type)
{
	if( !type ) return;

	ReflectionDatabase& map = ReflectionGetDatabase();
	const char* name = type->name;

	if( map.find(name) != map.end() )
	{
		LogAssert("Type '%s' already exists in the registry", name);
		return;
	}

	map[name] = type;

	if( !ReflectionIsComposite(type) )
		return;

	Class* klass = (Class*) type;
	RegisterClass(klass);
}

//-----------------------------------//

Type* ReflectionFindType(const char* name)
{
	ReflectionDatabase& types = ReflectionGetDatabase();
	
	ReflectionDatabase::iterator it = types.find(name);
	if( it == types.end() ) return nullptr;

	return it->second;
}

//-----------------------------------//

bool ReflectionIsEqual(const Type* t1, const Type* t2)
{
	return t1 == t2;
}

//-----------------------------------//

void EnumAddValue(Enum* enumeration, const char* name, int32 value)
{
	if( !enumeration ) return;

	EnumValuesMap& values = enumeration->values;
	values[name] = value;
}

//-----------------------------------//

int32 EnumGetValue(Enum* enumeration, const char* name)
{
	if( !enumeration ) return -1;
	EnumValuesMap& values = enumeration->values;
	
	EnumValuesMap::iterator it = values.find(name);
	if( it == values.end() ) return -1;
	
	return it->second;
}

//-----------------------------------//

const char* EnumGetValueName(Enum* enumeration, int32 value)
{
	if( !enumeration ) return nullptr;
	EnumValuesMap& values = enumeration->values;
	
	EnumValuesMap::iterator it = values.begin();
	
	for(; it != values.end(); ++it)
	{
		const char* name = it->first;
		if( value == it->second ) return name;
	}

	return nullptr;
}

//-----------------------------------//

void ClassAddField(Class* klass, Field* field)
{
	klass->fields.push_back(field);

	if( ClassGetFieldById(klass, field->id) )
	{
		LogAssert("Duplicate id found for field '%s' in '%s'", field->name, klass->name);
		return;
	}

	if( field->id == FieldInvalid )
	{
		LogAssert("Field id in '%s' is reserved", field->id);
		return;
	}

	ClassFieldIdMap& fieldIds = klass->fieldIds;
	fieldIds[field->id] = field;
}

//-----------------------------------//

ClassIdMap& ClassGetIdMap()
{
	static ClassIdMap s_ClassIds;
	return s_ClassIds;
}

//-----------------------------------//

Class* ClassGetById(ClassId id)
{
	ClassIdMap& classIds = ClassGetIdMap();
	ClassIdMap::iterator it = classIds.find(id);

	if( it == classIds.end() )
		return nullptr;

	return it->second;
}

//-----------------------------------//

Class* ClassGetType(const Object* object)
{
	if( !object ) return nullptr;
	return object->getType();
}

//-----------------------------------//

Field* ClassGetField(const Class* klass, const char* name)
{
	const std::vector<Field*>& fields = klass->fields;
	
	for(size_t i = 0; i < fields.size(); i++)
	{
		Field* field = fields[i];
		if(strcmp(field->name, name) == 0) return field;

		for(size_t u = 0; u < field->aliases.size(); u++)
		{
			const char* alias = field->aliases[u];
			if(strcmp(alias, name) == 0) return field;
		}
	}

	Class* parent = klass->parent;
	if(!parent) return nullptr;
	
	return ClassGetField(parent, name);
}

//-----------------------------------//

Field* ClassGetFieldById(Class* klass, FieldId id)
{
	ClassFieldIdMap& fieldIds = klass->fieldIds;

	ClassFieldIdMap::iterator it = fieldIds.find(id);

	if( it != fieldIds.end() )
		return it->second;

	if( klass->parent )
		return ClassGetFieldById(klass->parent, id);
	else
		return nullptr;
}

//-----------------------------------//

void* ClassGetFieldAddress(const void* object, const Field* field)
{
	if( !object || !field ) return nullptr;
	
	return (uint8*) object + field->offset;
}

//-----------------------------------//

bool ClassInherits(const Class* klass, const Class* test)
{
	bool equals = ReflectionIsEqual(klass, test);

	if( klass->parent == nullptr )
		return equals;

	return equals || ClassInherits(klass->parent, test);
}

//-----------------------------------//

void* ClassCreateInstance(const Class* klass, Allocator* alloc)
{
	if( !klass ) return nullptr;

	if( ClassIsAbstract(klass) )
	{
		LogAssert("Could not create instance of abstract class '%s'", klass->name);
		return nullptr;
	}
	
	void* object = klass->create_fn(alloc);
	return object;
}

//-----------------------------------//

bool ClassIsAbstract(const Class* klass)
{
	if( !klass ) return false;
	
	return klass->create_fn == nullptr;
}

//-----------------------------------//

Field::Field()
	: type(nullptr)
	, id(FieldInvalid)
	, name(nullptr)
	, size(0)
	, offset(0)
	, pointer_size(0)
	, qualifiers(0)
	, setter(nullptr)
	, resize(nullptr)
	, serialize(nullptr)
{

}

//-----------------------------------//

bool FieldHasQualifier(const Field* field, FieldQualifier::Enum qual)
{
	if( !field ) return false;
	return (field->qualifiers & qual) != 0;
}

//-----------------------------------//

void FieldSetQualifier(Field* field, FieldQualifier::Enum qual)
{
	if( !field ) return;
	field->qualifiers |= qual;
}

//-----------------------------------//

void FieldSetSetter(Field* field, FieldSetterFunction fn)
{
	if( !field ) return;
	field->setter = fn;
}

//-----------------------------------//

#define PT(type) Primitive::PrimitiveType::type

#pragma TODO("Do not use static init for primitive types")

Primitive Primitive::s_bool(PT(Bool), "bool", sizeof(bool));
Primitive Primitive::s_int8(PT(Int8), "int8", sizeof(int8));
Primitive Primitive::s_uint8(PT(Uint8), "uint8", sizeof(uint8));
Primitive Primitive::s_uint16(PT(Uint16), "uint16", sizeof(uint16));
Primitive Primitive::s_int16(PT(Int16), "int16", sizeof(int16));
Primitive Primitive::s_int32(PT(Int32), "int32", sizeof(int32));
Primitive Primitive::s_uint32(PT(Uint32), "uint32", sizeof(uint32));
Primitive Primitive::s_int64(PT(Int64), "int64", sizeof(int64));
Primitive Primitive::s_uint64(PT(Uint64), "uint64", sizeof(uint64));
Primitive Primitive::s_float(PT(Float), "float", sizeof(float));
Primitive Primitive::s_string(PT(String), "string", sizeof(String));
Primitive Primitive::s_Vector3(PT(Vector3), "Vector3", sizeof(Vector3));
Primitive Primitive::s_Color(PT(Color), "Color", sizeof(Color));
Primitive Primitive::s_Quaternion(PT(Quaternion), "Quaternion", sizeof(Quaternion));

//-----------------------------------//

// Specializations for all known primitive types.
template<> Primitive* GetPrimitiveFromType<bool>() { return &Primitive::s_bool; }
template<> Primitive* GetPrimitiveFromType<int8>() { return &Primitive::s_int8; }
template<> Primitive* GetPrimitiveFromType<uint8>() { return &Primitive::s_uint8; }
template<> Primitive* GetPrimitiveFromType<int16>() { return &Primitive::s_int16; }
template<> Primitive* GetPrimitiveFromType<uint16>() { return &Primitive::s_uint16; }
template<> Primitive* GetPrimitiveFromType<int32>() { return &Primitive::s_int32; }
template<> Primitive* GetPrimitiveFromType<uint32>() { return &Primitive::s_uint32; }
template<> Primitive* GetPrimitiveFromType<int64>() { return &Primitive::s_int64; }
template<> Primitive* GetPrimitiveFromType<uint64>() { return &Primitive::s_uint64; }
template<> Primitive* GetPrimitiveFromType<float>() { return &Primitive::s_float; }
template<> Primitive* GetPrimitiveFromType<const char*>() { return &Primitive::s_string; }
template<> Primitive* GetPrimitiveFromType<String>() { return &Primitive::s_string; }
template<> Primitive* GetPrimitiveFromType<Vector3>() { return &Primitive::s_Vector3; }
template<> Primitive* GetPrimitiveFromType<Color>() { return &Primitive::s_Color; }
template<> Primitive* GetPrimitiveFromType<Quaternion>() { return &Primitive::s_Quaternion; }

//-----------------------------------//

NAMESPACE_CORE_END