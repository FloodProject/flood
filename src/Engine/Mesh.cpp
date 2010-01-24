	/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Mesh.h"

namespace vapor { namespace resources {

//-----------------------------------//

const std::string& Mesh::type = "Mesh";

//-----------------------------------//

ResourceGroup::Enum Mesh::getResourceGroup() const
{
	return ResourceGroup::Meshes;
}

//-----------------------------------//

} } // end namespaces