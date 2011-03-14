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
#include <vector>

namespace vapor {

//-----------------------------------//

typedef std::pair<const String, Field*> FieldsPair;

/**
 * This class provides types with a fast RTTI (Runtime Type Information)
 * system that will be used for fast dynamic type checking and reflection.
 * Main use is for serialization and property introspection.
 */

class CORE_API Class : public Type
{
public:

	Class(const String& name, int size);
	Class(const String& name, const Type& parent, int size);

	// Gets the field with the given name.
	Field* getField(const String& name) const;

	// Adds a field to the class.
	void addField(Field& field);

	// Adds a parent to the class.
	void addChild(Class& type);

	// Returns a new instance of this type.
	virtual void* createInstance(void* address = nullptr) const;

public:

	// Keeps track of the type fields (in insertion order).
	std::vector<Field*> fields;

	// Keeps track of the childs of the class.
	std::vector<Class*> childs;
};

//-----------------------------------//

} // end namespace