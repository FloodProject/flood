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

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Mesh, Resource)
END_CLASS()

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

void Mesh::appendRenderables( std::vector<RenderablePtr>& rends )
{
	if( !built )
		build();

	rends = renderables;
}

//-----------------------------------//

} // end namespace