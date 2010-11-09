/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "core/Class.h"

namespace vapor {

//-----------------------------------//

Class::Class(const std::string& name)
	: Type(MetaType::Class, name)
{ }

//-----------------------------------//

Class::Class(const std::string& name, const Type& parent)
	: Type(MetaType::Class, name, parent)
{ }

//-----------------------------------//

void Class::addField(Field& field)
{
	fields[field.name] = &field;
}

//-----------------------------------//

const FieldsMap& Class::getFields() const
{
	return fields;
}

//-----------------------------------//

Field* Class::getField(const std::string& name) const
{
	FieldsMap::const_iterator it = fields.find(name);

	if( it != fields.end() )
		return it->second;
	
	if( parent )
		return ((Class*) parent)->getField(name);
	else
		return nullptr;
}

//-----------------------------------//

void Class::notifyChanged(const Field& field) const
{
	onFieldChanged(field);
}

//-----------------------------------//

} // end namespace