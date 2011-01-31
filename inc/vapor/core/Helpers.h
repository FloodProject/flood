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
		virtual void* createInstance() const { return new name(); }				\
	};																			\

#define BEGIN_CLASS_COMMON(name)												\
	typedef name This;															\
	using namespace std;														\
	static bool init = false;													\
	if(init) return type;														\
	init = true;																\

#define BEGIN_CLASS_DEFINE(name)												\
	static Class& gs_Class##name = name::getStaticType();						\

#define BEGIN_CLASS(name)														\
	SUBCLASS_CLASS(name)														\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType() { 												\
	static _##name type(TOSTRING(name), sizeof(name));							\
	BEGIN_CLASS_COMMON(name)

#define BEGIN_CLASS_ABSTRACT(name)												\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType() { 												\
	static Class type(TOSTRING(name), sizeof(name));							\
	BEGIN_CLASS_COMMON(name)

#define BEGIN_CLASS_PARENT(name, parent)										\
	SUBCLASS_CLASS(name)														\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType()												\
	{ static _##name type(TOSTRING(name), parent::getStaticType(),				\
		sizeof(name));															\
	BEGIN_CLASS_COMMON(name)

#define BEGIN_CLASS_PARENT_ABSTRACT(name, parent)								\
	BEGIN_CLASS_DEFINE(name)													\
	Class& name::getStaticType()												\
	{ static Class type(TOSTRING(name), parent::getStaticType(),				\
		sizeof(name));															\
	BEGIN_CLASS_COMMON(name)

#define END_CLASS()																\
	return type; }																\

//-----------------------------------//

#define NAME_FIELD_SETTER(fieldName)											\
	_set_##_##fieldName

#define NFS(fieldName)															\
	&fieldName##Set::##NAME_FIELD_SETTER(fieldName)

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

#define FIELD_CLASS_PTR(fieldType, fieldName, ...)								\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Pointer);									\
	FIELD_COMMON(fieldType##Ptr, fieldName, __VA_ARGS__)

#define FIELD_CLASS_PTR_SETTER(fieldType, fieldName, setterName)				\
	FIELD_SETTER_CLASS(fieldType##Ptr, fieldName, setterName)					\
	FIELD_CLASS_PTR(fieldType, fieldName, NFS(fieldName))

#define FIELD_ENUM(fieldType, fieldName, ...)									\
	static Field fieldName(fieldType::getStaticType());							\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

#define FIELD_ENUM_SETTER(fieldType, fieldName, setterName)						\
	FIELD_SETTER_CLASS(fieldType##::Enum, fieldName, setterName)				\
	FIELD_ENUM(fieldType, fieldName, NFS(fieldName))

#define FIELD_VECTOR(fieldType, fieldName, ...)									\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Array);									\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

#define FIELD_VECTOR_PTR(fieldType, pointerType, fieldName, ... )				\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Array);									\
	fieldName.setQualifier(Qualifier::Pointer);									\
	fieldName.size = sizeof(pointerType);										\
	FIELD_COMMON(fieldType, fieldName, __VA_ARGS__)

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

//-----------------------------------//

#define DECLARE_ENUM() static vapor::Enum& getStaticType();

#define BEGIN_ENUM(enumName)													\
	vapor::Enum& enumName::getStaticType() {									\
	static vapor::Enum enuHmm(TOSTRING(enumName), sizeof(enumName));
	
#define ENUM(enumName)															\
	enuHmm.addValue(TOSTRING(enumName), (int)enumName);

#define END_ENUM() return enuHmm; }

//-----------------------------------//

} // end namespace