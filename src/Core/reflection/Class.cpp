/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Class.h"
#include <algorithm>

namespace vapor {

//-----------------------------------//

Class::Class(const String& name, int size)
	: Type(MetaType::Class, name, size)
{ }

//-----------------------------------//

Class::Class(const String& name, const Type& parent, int size)
	: Type(MetaType::Class, name, parent, size)
{ }

//-----------------------------------//

void Class::addField(Field& field)
{
	fields.push_back(&field);
}

//-----------------------------------//

Field* Class::getField(const String& name) const
{
	for(size_t i = 0; i < fields.size(); i++)
	{
		Field* field = fields[i];
		if(field->name == name) return field;
	}
	
	Class* parent = (Class*) Type::parent;

	if(!parent) return nullptr;
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