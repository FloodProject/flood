/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

// Type is in a static function to work around the undefined order of
// initialization of static objects between different translation units.

#define REFLECT_DECLARE_CLASS DECLARE_CLASS_

#define DECLARE_CLASS_()														\
public:																			\
	static Class& getStaticType();												\
	virtual const Class& getType() const { return getStaticType(); }

#define SUBCLASS_CLASS(name)													\
	class _##name : public Class {												\
	public:																		\
		_##name(const std::string& n, int sz) : Class(n, sz) {}					\
		_##name(const std::string& n, const Type& p, int sz)					\
			: Class(n, p, sz) {}												\
		virtual void* createInstance(void* address) const {						\
			if(address)	return new(address) name();								\
			else return new name(); }											\
	};																			\

#define BEGIN_CLASS_COMMON(name)												\
	typedef name This;															\
	using namespace std;														\
	static bool init = false;													\
	if(init) return type;														\
	init = true;																\

#define BEGIN_CLASS_DEFINE(name)												\
	static Class& gs_Class##name = name::getStaticType();						\

#define REFLECT_CLASS BEGIN_CLASS

#define BEGIN_CLASS(name)														\
	SUBCLASS_CLASS(name)														\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType() { 												\
	static _##name type(TOSTRING(name), sizeof(name));							\
	BEGIN_CLASS_COMMON(name)

#define REFLECT_ABSTRACT_CLASS BEGIN_CLASS_ABSTRACT

#define BEGIN_CLASS_ABSTRACT(name)												\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType() { 												\
	static Class type(TOSTRING(name), sizeof(name));							\
	BEGIN_CLASS_COMMON(name)

#define REFLECT_CHILD_CLASS BEGIN_CLASS_PARENT

#define BEGIN_CLASS_PARENT(name, parent)										\
	SUBCLASS_CLASS(name)														\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType()												\
	{ static _##name type(TOSTRING(name), parent::getStaticType(),				\
		sizeof(name));															\
	BEGIN_CLASS_COMMON(name)

#define REFLECT_ABSTRACT_CHILD_CLASS BEGIN_CLASS_PARENT_ABSTRACT

#define BEGIN_CLASS_PARENT_ABSTRACT(name, parent)								\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType()												\
	{ static Class type(TOSTRING(name), parent::getStaticType(),				\
		sizeof(name));															\
	BEGIN_CLASS_COMMON(name)

#define REFLECT_END END_CLASS

#define END_CLASS()																\
	return type; }																\

//-----------------------------------//

#define NAME_FIELD_SETTER(fieldName)											\
	_set_##_##fieldName

#define NFS(fieldName)															\
	&fieldName##Set::NAME_FIELD_SETTER(fieldName)

#define FIELD_SETTER(fieldType, fieldName, setterName)							\
	static void	NAME_FIELD_SETTER(fieldName) (void* vobj, void* vparam)			\
	{																			\
		This* obj = (This*) vobj;												\
		fieldType* param = (fieldType*) vparam;									\
		if( obj && param ) obj->set##setterName(*param);						\
	}																			\

#define FIELD_SETTER_CLASS(fieldType, fieldName, setterName)					\
	class fieldName##Set { public:												\
		FIELD_SETTER(fieldType, fieldName, setterName) };						\

//-----------------------------------//

#define FIELD_COMMON(fieldType, fieldName, ...)									\
	fieldName.size = sizeof(fieldType);											\
	fieldName.offset = offsetof(This, fieldName);								\
	fieldName.name = TOSTRING(fieldName);										\
	fieldName.setSetter(__VA_ARGS__);											\
	type.addField(fieldName);

#define FIELD_CLASS(fieldType, fieldName, ...)									\
	static Field fieldName(fieldType::getStaticType());							\
	FIELD_COMMON(fieldType, fieldName)

#define FIELD_CLASS_PTR(fieldType, pointerType, fieldName, pointerQual, ...)	\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::pointerQual);								\
	fieldName.pointerSize = sizeof(fieldType##Ptr);								\
	FIELD_COMMON(pointerType, fieldName, __VA_ARGS__)

#define FIELD_CLASS_PTR_SETTER(fieldType, pointerType, fieldName, pointerQual, setterName)	\
	FIELD_SETTER_CLASS(pointerType, fieldName, setterName)									\
	FIELD_CLASS_PTR(fieldType, pointerType, fieldName, pointerQual, NFS(fieldName))

#define FIELD_ENUM(fieldType, fieldName, ...)									\
	static Field fieldName(fieldType::getStaticType());							\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

#define FIELD_ENUM_SETTER(fieldType, fieldName, setterName)						\
	FIELD_SETTER_CLASS(fieldType::Enum, fieldName, setterName)					\
	FIELD_ENUM(fieldType, fieldName, NFS(fieldName))

#define FIELD_VECTOR(fieldType, fieldName, ...)									\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Array);									\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

#define FIELD_VECTOR_PTR(fieldType, pointerType, fieldName, pointerQual, ... )	\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Array);									\
	fieldName.setQualifier(Qualifier::pointerQual);								\
	fieldName.pointerSize = sizeof(pointerType);								\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)								\

#define FIELD_PRIMITIVE(fieldType, fieldName, ...)								\
	static Field fieldName(Primitive::_##fieldType);							\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

#define FIELD_PRIMITIVE_SETTER(fieldType, fieldName, setterName)				\
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName)						\
	FIELD_PRIMITIVE(fieldType, fieldName, NFS(fieldName))

#define FIELD_PRIMITIVE_CUSTOM(fieldType, fieldName, primType, ...)				\
	static Field fieldName(Primitive::_##primType);								\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

#define FIELD_PRIMITIVE_SETTER_CUSTOM(fieldType, fieldName, primType, setterName)	\
	FIELD_SETTER_CLASS(fieldType, fieldName, setterName)							\
	FIELD_PRIMITIVE_CUSTOM(fieldType, fieldName, primType, NFS(fieldName))

#define FIELD_READONLY(fieldName)												\
	fieldName.setQualifier(Qualifier::ReadOnly);

//-----------------------------------//

#define REFLECT_DECLARE_ENUM DECLARE_ENUM

#define DECLARE_ENUM() static vapor::Enum& getStaticType();

#define REFLECT_ENUM BEGIN_ENUM

#define BEGIN_ENUM(enumName)													\
	vapor::Enum& enumName::getStaticType() {									\
	static vapor::Enum enuHmm(TOSTRING(enumName), sizeof(enumName));

#define REFLECT_ENUM_ENTRY ENUM

#define ENUM(enumName)															\
	enuHmm.addValue(TOSTRING(enumName), (int)enumName);

#define REFLECT_ENUM_END END_ENUM

#define END_ENUM() return enuHmm; }

//-----------------------------------//

} // end namespace