/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "core/Field.h"

namespace vapor {

//-----------------------------------//

Field::Field( const Type& type )
	: type(type)
	, qualifiers(0)
	, setterFunction(nullptr)
{ }

//-----------------------------------//

void Field::setSetter( SetterFunctionPtr fn )
{
	setterFunction = fn;
}

//-----------------------------------//

void Field::setQualifier( byte qualifier )
{
	qualifiers |= qualifier;
}

//-----------------------------------//

bool Field::isPointer() const
{
	return qualifiers == Qualifier::Pointer;
}

//-----------------------------------//

} // end namespace