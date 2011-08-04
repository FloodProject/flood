/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_CORE_BEGIN

//-----------------------------------//

#define REFLECT_DECLARE_ENUM(name)                                      \
	Enum* name##GetType();

#define REFLECT_DEFINE_ENUM(name)                                       \
    static Enum* name##BuildType();                                     \
	Enum* name##GetType() {                                             \
		static Enum* _enum = name##BuildType();                         \
		return _enum; }                                                 \
	static Enum* gs_Enum##name = name##GetType();

#define REFLECT_ENUM(enumName)                                          \
	REFLECT_DEFINE_ENUM(enumName)                                       \
	static Enum* enumName##BuildType() {                                \
		typedef enumName This;                                          \
		static Enum _enum;                                              \
		_enum.name = TOSTRING(enumName);                                \
		_enum.size = sizeof(enumName);                                  \
		_enum.type = Type::Enumeration;                                 \

#define ENUM(value)                                                     \
 EnumAddValue(&_enum, TOSTRING(value), This::value);

#define REFLECT_ENUM_END()                                              \
	ReflectionRegisterType(&_enum);                                     \
	return &_enum; }

//-----------------------------------//

#define REFLECT_DECLARE_CLASS(className)                                \
	Class* className##GetType();

#define REFLECT_DECLARE_CLASS_FRIEND(className)                         \
	friend Class* className##BuildType();

#define REFLECT_DEFINE_CLASS(className)                                 \
	Class* className##GetType() {                                       \
		static Class* klass = className##BuildType();                   \
		return klass; }                                                 \
	static Class* gs_Class##className = className##GetType();

#define CREATE_CLASS_FACTORY(className)                                 \
	void* className##CreateFactory( Allocator* alloc ) {                \
		return Allocate(className, alloc); }

#define CREATE_CLASS_BUILD(className)                                   \
	Class* className##BuildType();                                      \
	REFLECT_DEFINE_CLASS(className);                                    \
	Class* className##BuildType() {                                     \
		typedef className This;                                         \
		using namespace std;                                            \
		static Class klass;                                             \
		klass.name = TOSTRING(className);                               \
		klass.size = sizeof(className);                                 \
		klass.type = Type::Composite;

#define REFLECT_ABSTRACT_CLASS(name)                                    \
	CREATE_CLASS_BUILD(name)                                            \

#define REFLECT_ABSTRACT_CHILD_CLASS(name, parentName)                  \
	REFLECT_ABSTRACT_CLASS(name)                                        \
	klass.parent = parentName##GetType();

#define REFLECT_CLASS(className)                                        \
	CREATE_CLASS_FACTORY(className)                                     \
	CREATE_CLASS_BUILD(className)                                       \
	klass.create_fn = className##CreateFactory;                         \

#define REFLECT_CHILD_CLASS(name, parentName)                           \
	REFLECT_CLASS(name)                                                 \
	klass.parent = parentName##GetType();

#define REFLECT_CLASS_END()                                             \
	ReflectionRegisterType(&klass);                                     \
	return &klass; }                                                    \

//-----------------------------------//

#define NAME_FIELD_SETTER(fieldName)                                    \
	_set_##_##fieldName

#define NFS(fieldName)                                                  \
	&fieldName##Set::NAME_FIELD_SETTER(fieldName)

#define FIELD_SETTER(fieldType, fieldName, setterName)				    \
	static void	NAME_FIELD_SETTER(fieldName) (void* vobj, void* vparam) \
	{																	\
		This* obj = (This*) vobj;										\
		fieldType* param = (fieldType*) vparam;							\
		if( obj && param ) obj->set##setterName(*param);				\
	}																	\

#define FIELD_SETTER_CLASS(fieldType, fieldName, setterName)            \
	struct fieldName##Set {                                             \
		FIELD_SETTER(fieldType, fieldName, setterName) };

//-----------------------------------//

#define FIELD_COMMON(fieldId, fieldType, fieldName, ...)                \
	fieldName.size = sizeof(fieldType);                                 \
	fieldName.offset = offsetof(This, fieldName);                       \
	fieldName.name = TOSTRING(fieldName);                               \
	fieldName.id = fieldId;                                             \
	FieldSetSetter(&fieldName, __VA_ARGS__);                            \
	ClassAddField(&klass, &fieldName);

#define FIELD_CLASS(fieldId, fieldType, fieldName, ...)                 \
	static Field fieldName;                                             \
	fieldName.type = fieldType##GetType();                              \
	FIELD_COMMON(fieldId, fieldType, fieldName)

#define FIELD_CLASS_PTR(fieldId, fieldType, pointerType, fieldName, pointerQual, ...)  \
	static Field fieldName;                                                   \
	fieldName.type = fieldType##GetType();                                    \
	FieldSetQualifier(&fieldName, FieldQualifier::pointerQual);               \
	fieldName.pointer_size = sizeof(pointerType);                             \
	FIELD_COMMON(fieldId, pointerType, fieldName, __VA_ARGS__)

#define FIELD_CLASS_PTR_SETTER(fieldId, fieldType, pointerType, fieldName, pointerQual, setterName)	\
	FIELD_SETTER_CLASS(pointerType, fieldName, setterName)									\
	FIELD_CLASS_PTR(fieldId, fieldType, pointerType, fieldName, pointerQual, NFS(fieldName))

#define FIELD_ENUM(fieldId, fieldType, fieldName, ...)                  \
	static Field fieldName;                                             \
	fieldName.type = fieldType##GetType();                              \
	FIELD_COMMON(fieldId, fieldType, fieldName, __VA_ARGS__)

#define FIELD_ENUM_SETTER(fieldId, fieldType, fieldName, setterName)    \
	FIELD_SETTER_CLASS(fieldType::Enum, fieldName, setterName)          \
	FIELD_ENUM(fieldId, fieldType, fieldName, NFS(fieldName))

#define FIELD_VECTOR(fieldId, fieldType, fieldName, ...)                \
	static Field fieldName;					                            \
	FieldSetQualifier(&fieldName, FieldQualifier::Array);               \
	FIELD_COMMON(fieldId, fieldType, fieldName, __VA_ARGS__)

#define FIELD_VECTOR_PTR(fieldId, fieldType, pointerType, fieldName, pointerQual, ... )	\
	static Field fieldName;                                             \
	FieldSetQualifier(&fieldName, FieldQualifier::Array);				\
	FieldSetQualifier(&fieldName, FieldQualifier::pointerQual);			\
	fieldName.pointer_size = sizeof(pointerType);						\
	fieldName.type = ReflectionGetType(fieldType);                      \
	FIELD_COMMON(fieldId, fieldType, fieldName, __VA_ARGS__)            \

#define FIELD_PRIMITIVE(fieldId, fieldType, fieldName, ...)             \
	static Field fieldName;                                             \
	fieldName.type = &Primitive::s_##fieldType;                         \
	FIELD_COMMON(fieldId, fieldType, fieldName, __VA_ARGS__)

#define FIELD_PRIMITIVE_SETTER(fieldId, fieldType, fieldName, setterName) \
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName)                  \
	FIELD_PRIMITIVE(fieldId, fieldType, fieldName, NFS(fieldName))

#define FIELD_PRIMITIVE_CUSTOM(fieldId, fieldType, fieldName, primType, ...) \
	static Field fieldName(Primitive::_##primType);                          \
	FIELD_COMMON(fieldId, fieldType, fieldName, __VA_ARGS__)

#define FIELD_PRIMITIVE_SETTER_CUSTOM(fieldId, fieldType, fieldName, primType, setterName) \
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName)                                   \
	FIELD_PRIMITIVE_CUSTOM(fieldId, fieldType, fieldName, primType, NFS(fieldName))

#define FIELD_READONLY(fieldName)                                       \
	FieldSetQualifier(&fieldName, FieldQualifier::ReadOnly);

#define FIELD_NO_SERIALIZE(fieldName)                                   \
	FieldSetQualifier(&fieldName, FieldQualifier::NoSerialize);

#define FIELD_ALIAS(fieldName, aliasName)                               \
	fieldName.aliases.push_back(aliasName);


//-----------------------------------//

NAMESPACE_CORE_END