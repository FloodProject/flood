/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Type.h"
#include "Core/Field.h"
#include "Event.h"

namespace vapor {

//-----------------------------------//

typedef std::map<std::string, Field*> FieldsMap;
typedef std::pair<const std::string, Field*> FieldsPair;

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking and reflection.
 * Main use is for serialization and property introspection.
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

	// Gets the fields of the class.
	const std::vector<Field*>& getFieldsVector() const;

	// Adds a field to the class.
	void addField(Field& field);

	// Adds a parent to the class.
	void addChild(Class& type);

	// Returns a new instance of this type.
	virtual void* createInstance(void* address = nullptr) const;

public:

	// Keeps track of type fields.
	FieldsMap fields;

	// Keeps track of the type fields (in insertion order).
	std::vector<Field*> fieldsVector;

	// Keeps track of the childs of the class.
	std::vector<Class*> childs;
};

//-----------------------------------//

} // end namespace