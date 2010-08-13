/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/core/Type.h"
#include "vapor/core/Field.h"

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

class VAPOR_API Class : public Type
{
public:

	Class(const std::string& name);
	Class(const std::string& name, const Type& parent);

	// Gets the fields of the class.
	const FieldsMap& getFields() const;

	// Adds a field to the class.
	void addField(Field& field);

protected:

	// Keeps track of type fields.
	FieldsMap fields;
};

//-----------------------------------//

// Type is in a static function to work around the undefined order of
// initialization of static objects between different translation units.

#define DECLARE_CLASS_()												\
public:																	\
	static Class& getType();											\
	virtual const Class& getInstanceType() const { return getType(); }

#define BEGIN_CLASS(name)												\
	Class& name::getType()												\
	{ static Class type(TOSTRING(name));

#define BEGIN_CLASS_PARENT(name, parent)								\
	Class& name::getType()												\
	{ static Class type(TOSTRING(name), parent::getType());

#define END_CLASS()														\
	return type; }

#define FIELD_COMMON(classType, fieldName)													\
	fieldName.offset = offsetof(classType, fieldName);					\
	fieldName.name = TOSTRING(fieldName);								\
	type.addField(fieldName);

#define FIELD_CLASS(classType, fieldType, fieldName)					\
	static Field fieldName(fieldType::getType());						\
	FIELD_COMMON(classType, fieldName)

#define FIELD_ENUM(classType, fieldType, fieldName)						\
	static Field fieldName(fieldType::getType());						\
	FIELD_COMMON(classType, fieldName)

#define FIELD_PRIMITIVE(classType, fieldType, fieldName)				\
	static Field fieldName(Primitive::_##fieldType);					\
	FIELD_COMMON(classType, fieldName)

//-----------------------------------//

} // end namespace
