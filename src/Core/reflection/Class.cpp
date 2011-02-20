/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "core/Class.h"

namespace vapor {

//-----------------------------------//

Class::Class(const std::string& name, int size)
	: Type(MetaType::Class, name, size)
{ }

//-----------------------------------//

Class::Class(const std::string& name, const Type& parent, int size)
	: Type(MetaType::Class, name, parent, size)
{ }

//-----------------------------------//

void Class::addField(Field& field)
{
	fields[field.name] = &field;
	fieldsVector.push_back(&field);
}

//-----------------------------------//

const FieldsMap& Class::getFields() const
{
	return fields;
}

//-----------------------------------//

const std::vector<Field*>& Class::getFieldsVector() const
{
	return fieldsVector;
}

//-----------------------------------//

Field* Class::getField(const std::string& name) const
{
	FieldsMap::const_iterator it = fields.find(name);

	if( it != fields.end() )
		return it->second;

	if( !parent ) return nullptr;

	Class* parent = (Class*) Type::parent;
	return parent->getField(name);
}

//-----------------------------------//

void Class::addChild(Class& type)
{
	childs.push_back(&type);
}

//-----------------------------------//

void* Class::createInstance(void* address) const
{
	return nullptr;
}

//-----------------------------------//

} // end namespace