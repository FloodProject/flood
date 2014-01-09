/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Log.h"
#include "Core/Math/Hash.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CLASS(Object)
REFLECT_CLASS_END()

//-----------------------------------//

bool ReflectionDatabase::registerType(Type* type)
{
	if (!type) return false;

	const char* name = type->name;

	if (types.find(name) != types.end())
	{
		LogAssert("Type '%s' already exists in the database", name);
		return false;
	}

	types[name] = type;

	if(!type->isComposite())
		return true;

	Class* klass = (Class*) type;
	klass->registerClass();

	return true;
}

//-----------------------------------//

Type::Type()
{
}

//-----------------------------------//

Type::Type(TypeKind kind, const char* name, uint16 size)
	: kind(kind), name(name), size(size), serialize(nullptr)
{
}

//-----------------------------------//

Type::~Type()
{
}

//-----------------------------------//

bool Type::isPrimitive()
{
	return kind == TypeKind::Primitive;
}

//-----------------------------------//

bool Type::isComposite()
{
	return kind == TypeKind::Composite;
}

//-----------------------------------//

bool Type::isEnum()
{
	return kind == TypeKind::Enumeration;
}

//-----------------------------------//

ReflectionDatabase& ReflectionDatabase::GetDatabase()
{
	static ReflectionDatabase s_ReflectionDatabase;
	return s_ReflectionDatabase;
}

//-----------------------------------//

Type* ReflectionDatabase::findType(const char* name)
{
	TypeMap::iterator it = types.find(name);
	if (it == types.end()) return nullptr;

	return it->second;
}

//-----------------------------------//

bool Type::isEqual(const Type* type) const
{
	return this == type;
}

//-----------------------------------//

bool Type::operator==(const Type& type) const
{
	return this == &type;
}

//-----------------------------------//

Class::Class()
	 : parent(nullptr)
	 , create_fn(nullptr)
{
}

//-----------------------------------//

Class* Class::getParent()
{
	return parent;
} 

//-----------------------------------//

bool Class::hasParent()
{
	return parent != nullptr;
} 

//-----------------------------------//

void Class::registerClass()
{
	id = calculateId();

	//LogDebug("Registering class: '%s' with id '%u'", klass->name, klass->id);

	// Register as child class in the parent class.
	if (parent) parent->childs.Push(this);

	// Register the class id in the map.
	ClassIdMap& ids = Class::GetIdMap();

	if( ids.Find(id) != ids.End() )
	{
		LogError("Class with the same id already exists: '%s'", name);
		return;
	}

	ids[id] = this;
}

//-----------------------------------//

void Class::addField(Field* field)
{
	fields.Push(field);

	if (getFieldById(field->id))
	{
		LogAssert("Duplicate id found for field '%s' in '%s'",
			field->name, name);
		return;
	}

	if( field->id == Field::FieldInvalid )
	{
		LogAssert("Field id in '%s' is reserved", field->id);
		return;
	}

	fieldIds[field->id] = field;
}

//-----------------------------------//

ClassIdMap& Class::GetIdMap()
{
	static ClassIdMap s_ClassIds;
	return s_ClassIds;
}

//-----------------------------------//

Class* Class::getById(ClassId id)
{
	ClassIdMap& classIds = GetIdMap();
    ClassIdMap::Iterator it = classIds.Find(id);

	return (it == classIds.End())? nullptr : it->second;
}

//-----------------------------------//

Class* Class::GetType(const Object* object)
{
	if (!object ) return nullptr;
	return object->getType();
}

//-----------------------------------//

Field* Class::getField(const char* name)
{
	for(auto& field : fields)

	{
		if(strcmp(field->name, name) == 0) return field;

		for(auto& alias : field->aliases)
			if(strcmp(alias, name) == 0) return field;
	}

	if (!parent) return nullptr;
	
	return parent->getField(name);
}

//-----------------------------------//

Field* Class::getFieldById(FieldId id)
{
    ClassFieldIdMap::Iterator it = fieldIds.Find(id);

    if( it != fieldIds.End() )
		return it->second;

	if (parent)
		return parent->getFieldById(id);
	else
		return nullptr;
}

//-----------------------------------//

void* Class::GetFieldAddress(const void* object, const Field* field)
{
	if( !object || !field ) return nullptr;
	
	return (uint8*) object + field->offset;
}

//-----------------------------------//

bool Class::inherits(const Class* test)
{
	bool equals = isEqual(test);

	if (!parent)
		return equals;

	return equals || parent->inherits(test);
}

//-----------------------------------//

void* Class::createInstance(Allocator* allocator)
{
	if (isAbstract())
	{
		LogAssert("Could not create instance of abstract class '%s'", name);
		return nullptr;
	}
	
	void* object = create_fn(allocator);
	return object;
}

//-----------------------------------//

ClassId Class::calculateId()
{
	uint32 hash = HashMurmur2(0xBEEF, (uint8*) name, strlen(name));
	return (ClassId) hash;
}

//-----------------------------------//

bool Class::isAbstract()
{
	return create_fn == nullptr;
}

//-----------------------------------//

Field::Field()
	: type(nullptr)
	, id(FieldInvalid)
	, name(nullptr)
	, size(0)
	, offset(0)
	, pointer_size(0)
	, qualifiers((FieldQualifier)0)
	, setter(nullptr)
	, resize(nullptr)
	, serialize(nullptr)
{

}

//-----------------------------------//

bool Field::hasQualifier(FieldQualifier qual) const
{
	return ((uint16)qualifiers & (uint16)qual) != 0;
}

//-----------------------------------//

void Field::setQualifier(FieldQualifier qual)
{
	*((uint16*)&qualifiers) |= (uint16)qual;
}

//-----------------------------------//

void Field::setSetter(FieldSetterFunction fn)
{
	setter = fn;
}

//-----------------------------------//

Primitive::Primitive()
{
}

//-----------------------------------//

Primitive::Primitive(PrimitiveTypeKind kind, 
					 const char* name, uint16 size)
	: Type(TypeKind::Primitive, name, size), kind(kind)
{
}

//-----------------------------------//

PrimitiveBuiltins::PrimitiveBuiltins()
	: p_bool(PrimitiveTypeKind::Bool, "bool", sizeof(bool))
	, p_int8(PrimitiveTypeKind::Int8, "int8", sizeof(int8))
	, p_uint8(PrimitiveTypeKind::Uint8, "uint8", sizeof(uint8))
	, p_uint16(PrimitiveTypeKind::Uint16, "uint16", sizeof(uint16))
	, p_int16(PrimitiveTypeKind::Int16, "int16", sizeof(int16))
	, p_int32(PrimitiveTypeKind::Int32, "int32", sizeof(int32))
	, p_uint32(PrimitiveTypeKind::Uint32, "uint32", sizeof(uint32))
	, p_int64(PrimitiveTypeKind::Int64, "int64", sizeof(int64))
	, p_uint64(PrimitiveTypeKind::Uint64, "uint64", sizeof(uint64))
	, p_float(PrimitiveTypeKind::Float, "float", sizeof(float))
	, p_string(PrimitiveTypeKind::String, "string", sizeof(String))
    , p_utf8string(PrimitiveTypeKind::UTF8String, "utf8string", sizeof(UTF8String))
	, p_Vector3(PrimitiveTypeKind::Vector3, "Vector3", sizeof(Vector3))
	, p_Color(PrimitiveTypeKind::Color, "Color", sizeof(Color))
	, p_Quaternion(PrimitiveTypeKind::Quaternion, "Quaternion", sizeof(Quaternion))
{}

PrimitiveBuiltins& PrimitiveBuiltins::GetBuiltins()
{
	static PrimitiveBuiltins g_PrimitiveBuiltins;
	return g_PrimitiveBuiltins;
}

//-----------------------------------//

// Specializations for all known primitive types.
template<> Primitive* GetPrimitiveFromType<bool>() { return &PrimitiveGetBuiltins().p_bool; }
template<> Primitive* GetPrimitiveFromType<int8>() { return &PrimitiveGetBuiltins().p_int8; }
template<> Primitive* GetPrimitiveFromType<uint8>() { return &PrimitiveGetBuiltins().p_uint8; }
template<> Primitive* GetPrimitiveFromType<int16>() { return &PrimitiveGetBuiltins().p_int16; }
template<> Primitive* GetPrimitiveFromType<uint16>() { return &PrimitiveGetBuiltins().p_uint16; }
template<> Primitive* GetPrimitiveFromType<int32>() { return &PrimitiveGetBuiltins().p_int32; }
template<> Primitive* GetPrimitiveFromType<uint32>() { return &PrimitiveGetBuiltins().p_uint32; }
template<> Primitive* GetPrimitiveFromType<int64>() { return &PrimitiveGetBuiltins().p_int64; }
template<> Primitive* GetPrimitiveFromType<uint64>() { return &PrimitiveGetBuiltins().p_uint64; }
template<> Primitive* GetPrimitiveFromType<float>() { return &PrimitiveGetBuiltins().p_float; }
template<> Primitive* GetPrimitiveFromType<const char*>() { return &PrimitiveGetBuiltins().p_string; }
template<> Primitive* GetPrimitiveFromType<String>() { return &PrimitiveGetBuiltins().p_string; }
template<> Primitive* GetPrimitiveFromType<UTF8String>() { return &PrimitiveGetBuiltins().p_utf8string; }
template<> Primitive* GetPrimitiveFromType<Vector3>() { return &PrimitiveGetBuiltins().p_Vector3; }
template<> Primitive* GetPrimitiveFromType<Color>() { return &PrimitiveGetBuiltins().p_Color; }
template<> Primitive* GetPrimitiveFromType<Quaternion>() { return &PrimitiveGetBuiltins().p_Quaternion; }

//-----------------------------------//

void Enum::addValue(const char* name, int32 value)
{
	values[name] = value;
}

//-----------------------------------//

int32 Enum::getValue(const char* name)
{
	EnumValuesMap::iterator it = values.Find(name);
	if (it == values.End()) return -1;
	
	return it->second;
}

//-----------------------------------//

const char* Enum::getValueName(int32 value)
{
	EnumValuesMap::iterator it = values.Begin();
	
	for(; it != values.End(); ++it)
	{
		const char* name = it->first;
		if( value == it->second ) return name;
	}

	return nullptr;
}

//-----------------------------------//

NAMESPACE_CORE_END