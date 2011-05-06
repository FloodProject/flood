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

} // end namespace