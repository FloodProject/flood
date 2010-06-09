	/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Mesh.h"
#include "vapor/render/Renderable.h"

namespace vapor { namespace resources {

//-----------------------------------//

Mesh::Mesh()
	: built(false)
{ }

//-----------------------------------//

bool Mesh::isBuilt() const
{
	return built;
}

//-----------------------------------//

} } // end namespaces