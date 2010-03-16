/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Target.h"

namespace vapor { namespace render {

//-----------------------------------//

math::Vector2i Settings::getSize() const
{
	return math::Vector2i(width, height);
}

//-----------------------------------//

} } // end namespaces
