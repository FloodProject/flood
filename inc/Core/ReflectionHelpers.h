/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Memory.h"
#include "Core/Reflection.h"
#include "Core/Math/Color.h"
#include "Core/Math/Quaternion.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

#define REFLECT_DECLARE_ENUM(name) \
	Enum* name##GetType();

#define REFLECT_DEFINE_ENUM(name) \
	static Enum* name##BuildType(); \
	Enum* name##GetType() { \
		static Enum* _enum = name##BuildType(); \
		return _enum; } \
	static Enum* gs_Enum##name = name##GetType();

#define REFLECT_ENUM(enumName) \
	REFLECT_DEFINE_ENUM(enumName) \
	static Enum* enumName##BuildType() { \
		typedef enumName This; \
		static Enum _enum; \
		_enum.name = TOSTRING(enumName); \
		_enum.size = sizeof(enumName); \
		_enum.kind = TypeKind::Enumeration; \

#define ENUM(value) \
 _enum.addValue(TOSTRING(value), (int32) This::value);

#define REFLECT_ENUM_END() \
	ReflectionDatabase::GetDatabase().registerType(&_enum); \
	return &_enum; }

//-----------------------------------//

#define REFLECT_DECLARE_CLASS(className) \
	Class* className##GetType();

#define REFLECT_DECLARE_CLASS_FRIEND(className) \
	friend Class* className##BuildType();

#define REFLECT_DEFINE_CLASS(className) \
	Class* className##GetType() { \
		static Class* klass = className##BuildType(); \
		return klass; } \
	static Class* gs_Class##className = className##GetType();

#define CREATE_CLASS_FACTORY(className) \
	void* className##CreateFactory( Allocator* alloc ) { \
		return Allocate(alloc, className); }

#define CREATE_CLASS_BUILD(className) \
	Class* className##BuildType(); \
	REFLECT_DEFINE_CLASS(className); \
	Class* className##BuildType() { \
		typedef className This; \
		using namespace std; \
		static Class klass; \
		klass.name = TOSTRING(className); \
		klass.size = sizeof(className); \
		klass.kind = TypeKind::Composite;

#define REFLECT_CLASS_SET_SERIALIZER(fn) \
	klass.serialize = fn;

#define REFLECT_ABSTRACT_CLASS(name) \
	CREATE_CLASS_BUILD(name) \

#define REFLECT_ABSTRACT_CHILD_CLASS(name, parentName) \
	REFLECT_ABSTRACT_CLASS(name) \
	klass.parent = parentName##GetType();

#define REFLECT_CLASS(className) \
	CREATE_CLASS_FACTORY(className) \
	CREATE_CLASS_BUILD(className) \
	klass.create_fn = className##CreateFactory; \

#define REFLECT_CHILD_CLASS(name, parentName) \
	REFLECT_CLASS(name) \
	klass.parent = parentName##GetType();

#define REFLECT_CLASS_END() \
	ReflectionDatabase::GetDatabase().registerType(&klass); \
	return &klass; } \

//-----------------------------------//

#define FIELD_SETTER_NAME(fieldName) \
	&fieldName##Set::Set##fieldName

#define FIELD_SETTER(fieldType, fieldName, setterName) \
	static void Set##fieldName (void* vobj, void* vparam) { \
		This* obj = (This*) vobj; \
		fieldType* param = (fieldType*) vparam; \
		if( obj && param ) obj->set##setterName(*param); \
	} \

#define FIELD_SETTER_CLASS(fieldType, fieldName, setterName) \
	struct fieldName##Set { \
		FIELD_SETTER(fieldType, fieldName, setterName) \
	};

//-----------------------------------//

#define FIELD_RESIZER_NAME(fieldName) \
	&fieldName##Resize::Resize##fieldName

#define FIELD_RESIZER(fieldType, fieldName) \
	static void* Resize##fieldName (void* obj, size_t size) { \
		Vector<fieldType>* array = (Vector<fieldType>*) obj; \
		array->Resize(size); \
		return &array->Front(); \
	}

#define FIELD_RESIZER_CLASS(fieldType, fieldName) \
	struct fieldName##Resize { \
		FIELD_RESIZER(fieldType, fieldName) \
	};

//-----------------------------------//

#define FIELD_COMMON(fieldId, fieldType, fieldName) \
	fieldName.size = sizeof(fieldType); \
	fieldName.offset = offsetof(This, fieldName); \
	fieldName.name = TOSTRING(fieldName); \
	fieldName.id = fieldId; \
	klass.addField(&fieldName);

#define FIELD_SET_SERIALIZER(fieldName, fn) \
	fieldName.serialize = fn;

#define FIELD_CLASS(fieldId, fieldType, fieldName) \
	static Field fieldName; \
	fieldName.type = fieldType##GetType(); \
	FIELD_COMMON(fieldId, fieldType, fieldName)

#define FIELD_CLASS_PTR(fieldId, fieldType, pointerType, fieldName, pointerQual) \
	static Field fieldName; \
	fieldName.type = fieldType##GetType(); \
	fieldName.setQualifier(FieldQualifier::pointerQual); \
	fieldName.pointer_size = sizeof(pointerType); \
	FIELD_COMMON(fieldId, pointerType, fieldName)

#define FIELD_CLASS_PTR_SETTER(fieldId, fieldType, pointerType, fieldName, pointerQual, setterName) \
	FIELD_SETTER_CLASS(pointerType, fieldName, setterName) \
	FIELD_CLASS_PTR(fieldId, fieldType, pointerType, fieldName, pointerQual) \
	fieldName.setSetter(FIELD_SETTER_NAME(fieldName));

#define FIELD_ENUM(fieldId, fieldType, fieldName) \
	static Field fieldName; \
	fieldName.type = fieldType##GetType(); \
	FIELD_COMMON(fieldId, fieldType, fieldName)

#define FIELD_ENUM_SETTER(fieldId, fieldType, fieldName, setterName) \
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName) \
	FIELD_ENUM(fieldId, fieldType, fieldName) \
	fieldName.setSetter(FIELD_SETTER_NAME(fieldName));

#define FIELD_VECTOR(fieldId, fieldType, fieldName) \
	static Field fieldName; \
	fieldName.setQualifier(FieldQualifier::Array); \
	fieldName.type = fieldType##GetType(); \
	FIELD_RESIZER_CLASS(fieldType, fieldName) \
	fieldName.resize = FIELD_RESIZER_NAME(fieldName); \
	FIELD_COMMON(fieldId, fieldType, fieldName)

#define FIELD_VECTOR_PTR(fieldId, fieldType, pointerType, fieldName, pointerQual ) \
	static Field fieldName; \
	fieldName.setQualifier(FieldQualifier::Array); \
	fieldName.setQualifier(FieldQualifier::pointerQual); \
	fieldName.pointer_size = sizeof(pointerType); \
	fieldName.type = fieldType##GetType(); \
	FIELD_COMMON(fieldId, fieldType, fieldName) \

#define FIELD_PRIMITIVE(fieldId, fieldType, fieldName) \
	static Field fieldName; \
	fieldName.type = &PrimitiveBuiltins::GetBuiltins().p_##fieldType; \
	FIELD_COMMON(fieldId, fieldType, fieldName)

#define FIELD_PRIMITIVE_SETTER(fieldId, fieldType, fieldName, setterName) \
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName) \
	FIELD_PRIMITIVE(fieldId, fieldType, fieldName) \
	fieldName.setSetter(FIELD_SETTER_NAME(fieldName));

#define FIELD_PRIMITIVE_CUSTOM(fieldId, fieldType, fieldName, primType) \
	static Field fieldName; \
	fieldName.type = &Primitive::s_##fieldType; \
	FIELD_COMMON(fieldId, fieldType, fieldName)

#define FIELD_PRIMITIVE_SETTER_CUSTOM(fieldId, fieldType, fieldName, primType, setterName) \
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName) \
	FIELD_PRIMITIVE_CUSTOM(fieldId, fieldType, fieldName, primType) \
	fieldName.setSetter(FIELD_SETTER_NAME(fieldName));

#define FIELD_READONLY(fieldName) \
	fieldName.setQualifier(FieldQualifier::ReadOnly);

#define FIELD_NO_SERIALIZE(fieldName) \
	fieldName.setQualifier(FieldQualifier::NoSerialize);

#define FIELD_ALIAS(fieldName, aliasName) \
	fieldName.aliases.Push(#aliasName);

//-----------------------------------//

// In the general case we don't know the type.
template<typename T>
Primitive* GetPrimitiveFromType() { return nullptr; };

// Specializations for all known primitive types.
template<> Primitive* GetPrimitiveFromType<bool>();
template<> Primitive* GetPrimitiveFromType<int8>();
template<> Primitive* GetPrimitiveFromType<uint8>();
template<> Primitive* GetPrimitiveFromType<int16>();
template<> Primitive* GetPrimitiveFromType<uint16>();
template<> Primitive* GetPrimitiveFromType<int32>();
template<> Primitive* GetPrimitiveFromType<uint32>();
template<> Primitive* GetPrimitiveFromType<int64>();
template<> Primitive* GetPrimitiveFromType<uint64>();
template<> Primitive* GetPrimitiveFromType<float>();
template<> Primitive* GetPrimitiveFromType<const char*>();
template<> Primitive* GetPrimitiveFromType<String>();
template<> Primitive* GetPrimitiveFromType<Vector3>();
template<> Primitive* GetPrimitiveFromType<Color>();
template<> Primitive* GetPrimitiveFromType<Quaternion>();

template<typename T>
Primitive* GetPrimitiveFromType(const T& type)
{
	return GetPrimitiveFromType<T>();
};

//-----------------------------------//

NAMESPACE_CORE_END