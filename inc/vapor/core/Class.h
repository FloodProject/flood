/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "core/Type.h"
#include "core/Field.h"
#include "Event.h"

namespace vapor {

//-----------------------------------//

typedef std::map<std::string, Field*> FieldsMap;
typedef std::pair<const std::string, Field*> FieldsPair;

//-----------------------------------//

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking, reflection,
 * serialization and also property editing in the editor.
 */

class CORE_API Class : public Type
{
public:

	Class(const std::string& name, int size);
	Class(const std::string& name, const Type& parent, int size);

	// Gets the field with the given name.
	Field* getField(const std::string& name) const;

	// Gets the fields of the class.
	const FieldsMap& getFields() const;

	// Adds a field to the class.
	void addField(Field& field);

	// Gets a value in the field.
	template<typename T>
	bool getFieldValue( const std::string& name, const void* obj, T& value ) const
	{
		Field* field = getField(name);
		if( !field ) return false;
		
		value = field->get<T>(obj);
		return true;
	}

	// Sets a value in the field.
	template<typename T>
	bool setFieldValue( const std::string& name, const void* obj, const T& value )
	{
		Field* field = getField(name);
		if( !field ) return false;

		SetterFunctionPtr setFn = field->setterFunction;
		
		if(setFn)
			setFn((void*) obj, (void*) &value);
		else
			field->set<T>(obj, value);

		notifyChanged(*field);

		return true;
	}

	// Returns a new instance of this type.
	virtual void* createInstance() const;

	// Sent when a field value is changed.
	Event1<const Field&> onFieldChanged;

protected:

	// Notifies watchers that the field changed.
	void notifyChanged(const Field& field) const;

	// Keeps track of type fields.
	FieldsMap fields;
};

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

#define BEGIN_CLASS(name)														\
	SUBCLASS_CLASS(name)														\
	static Class& _ = name::getStaticType();									\
	Class& name::getStaticType()												\
	{ static _##name type(TOSTRING(name), sizeof(name));

#define BEGIN_CLASS_ABSTRACT(name)												\
	static Class& _ = name::getStaticType();									\
	Class& name::getStaticType()												\
	{ static Class type(TOSTRING(name), sizeof(name));

#define BEGIN_CLASS_PARENT(name, parent)										\
	SUBCLASS_CLASS(name)														\
	static Class& _ = name::getStaticType();									\
	Class& name::getStaticType()												\
	{ static _##name type(TOSTRING(name), parent::getStaticType(),				\
		sizeof(name));

#define BEGIN_CLASS_PARENT_ABSTRACT(name, parent)								\
	static Class& _ = name::getStaticType();									\
	Class& name::getStaticType()												\
	{ static Class type(TOSTRING(name), parent::getStaticType(),				\
		sizeof(name));

#define END_CLASS()																\
	return type; }																\


#define NAME_FIELD_SETTER(classType, fieldName)									\
	_set_##classType##_##fieldName

#define NFS(classType, fieldName)												\
	&fieldName##Set::_set_##classType##_##fieldName

#define FIELD_SETTER(classType, fieldType, fieldName, setterName)				\
	static void																	\
	NAME_FIELD_SETTER(classType, fieldName)	(void* vobj, void* vparam) {		\
		classType* obj = (classType*) vobj;										\
		fieldType* param = (fieldType*) vparam;									\
		if( obj && param ) obj->set##setterName(*param); }						\

#define FIELD_SETTER_CLASS(classType, fieldType, fieldName, setterName)			\
	class fieldName##Set {														\
	public:																		\
		FIELD_SETTER(classType, fieldType, fieldName, setterName) };			\

#define FIELD_COMMON(classType, fieldName, ...)									\
	fieldName.offset = offsetof(classType, fieldName);							\
	fieldName.name = TOSTRING(fieldName);										\
	fieldName.setSetter(__VA_ARGS__);											\
	type.addField(fieldName);

#define FIELD_CLASS(classType, fieldType, fieldName, ...)						\
	static Field fieldName(fieldType::getStaticType());							\
	FIELD_COMMON(classType, fieldName)

#define FIELD_CLASS_PTR(classType, fieldType, fieldName, ...)					\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Pointer);									\
	FIELD_COMMON(classType, fieldName, __VA_ARGS__)

#define FIELD_CLASS_PTR_SETTER(classType, fieldType, fieldName, setterName)		\
	FIELD_SETTER_CLASS(classType, fieldType##Ptr, fieldName, setterName)		\
	FIELD_CLASS_PTR(classType, fieldType, fieldName, NFS(classType, fieldName))

#define FIELD_ENUM(classType, fieldType, fieldName, ...)						\
	static Field fieldName(fieldType::getStaticType());							\
	FIELD_COMMON(classType, fieldName, __VA_ARGS__)

#define FIELD_ENUM_SETTER(classType, fieldType, fieldName, setterName)			\
	FIELD_SETTER_CLASS(classType, fieldType##::Enum, fieldName, setterName)		\
	FIELD_ENUM(classType, fieldType, fieldName, NFS(classType, fieldName))

#define FIELD_VECTOR(classType, fieldType, fieldName, ...)						\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Array);									\
	FIELD_COMMON(classType, fieldName, __VA_ARGS__)

#define FIELD_VECTOR_PTR(classType, fieldType, pointerType, fieldName, ... )	\
	static Field fieldName(fieldType::getStaticType());							\
	fieldName.setQualifier(Qualifier::Array);									\
	fieldName.setQualifier(Qualifier::Pointer);									\
	fieldName.size = sizeof(pointerType);										\
	FIELD_COMMON(classType, fieldName, __VA_ARGS__)

#define FIELD_PRIMITIVE(classType, fieldType, fieldName, ...)					\
	static Field fieldName(Primitive::_##fieldType);							\
	FIELD_COMMON(classType, fieldName, __VA_ARGS__)

#define FIELD_PRIMITIVE_SETTER(classType, fieldType, fieldName, setterName)		\
	FIELD_SETTER_CLASS(classType, fieldType, fieldName, setterName)				\
	FIELD_PRIMITIVE(classType, fieldType, fieldName, NFS(classType, fieldName))

//-----------------------------------//

} // end namespace
