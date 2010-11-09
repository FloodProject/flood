/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "core/Field.h"

namespace vapor {

//-----------------------------------//

Field::Field( const Type& type )
	: type(type)
	, pointer(false)
{ }

//-----------------------------------//

bool Field::isPointer() const
{
	return pointer;
}

//-----------------------------------//

} // end namespace