/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/core/Class.h"

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

void Class::notifyChanged(const Field& field) const
{
	if( onFieldChanged.empty() )
		return;
	
	onFieldChanged(field);
}

//-----------------------------------//

} // end namespace