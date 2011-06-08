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

NAMESPACE_BEGIN

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

ReflectionTypeMap& ReflectionGetTypeMap()
{
	static ReflectionTypeMap s_ReflectionTypes;
	return s_ReflectionTypes;
}

//-----------------------------------//

void ReflectionRegisterType(Type* type)
{
	if( !type ) return;

	const char* name = type->name;

	ReflectionTypeMap& map = ReflectionGetTypeMap();

	if( map.find(name) != map.end() )
	{
		assert(0 && "Type already exists in the registry");
		return;
	}

	map[name] = type;

	if( !ReflectionIsComposite(type) )
		return;
	
	Class* klass = (Class*) type;
	Class* parent = klass->parent;

	if( !parent ) return;
	parent->childs.push_back(klass);
}

//-----------------------------------//

Type* ReflectionFindType(const char* name)
{
	ReflectionTypeMap& types = ReflectionGetTypeMap();
	
	ReflectionTypeMap::iterator it = types.find(name);
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
	
	for(; it != values.end(); it++)
	{
		const char* name = it->first;
		if( value == it->second ) return name;
	}

	return nullptr;
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
	}

	Class* parent = klass->parent;
	if(!parent) return nullptr;
	
	return ClassGetField(parent, name);
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

	assert( !ClassIsAbstract(klass) );
	
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
	field->setter_fn = fn;
}

//-----------------------------------//

#define PT(type) Primitive::PrimitiveType::type

Primitive Primitive::s_bool(PT(Bool), "bool", sizeof(bool));
Primitive Primitive::s_int32(PT(Int32), "int32", sizeof(int32));
Primitive Primitive::s_uint32(PT(Uint32), "uint32", sizeof(uint32));
Primitive Primitive::s_float(PT(Float), "float", sizeof(float));
Primitive Primitive::s_string(PT(String), "string", sizeof(String));
Primitive Primitive::s_Vector3(PT(Vector3), "Vector3", sizeof(Vector3));
Primitive Primitive::s_Color(PT(Color), "Color", sizeof(Color));
Primitive Primitive::s_Quaternion(PT(Quaternion), "Quaternion", sizeof(Quaternion));
Primitive Primitive::s_Bitfield(PT(Bitfield), "Bitfield", sizeof(uint32));

//-----------------------------------//

NAMESPACE_END