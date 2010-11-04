/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/core/Field.h"

namespace vapor {

//-----------------------------------//

Field::Field( const Type& _type )
	: type(_type)
	, pointer(false)
{ }

//-----------------------------------//

bool Field::isPointer() const
{
	return pointer;
}

//-----------------------------------//

} // end namespace