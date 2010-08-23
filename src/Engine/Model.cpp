/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Model.h"
#include "vapor/resources/Mesh.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Model, Geometry)
	FIELD_CLASS_PTR(Model, Mesh, mesh) 
END_CLASS()

//-----------------------------------//

Model::Model()
	: modelBuilt(false)
{ }

//-----------------------------------//

Model::Model( const MeshPtr& mesh )
	: mesh(mesh)
	, modelBuilt(false)
{ }

//-----------------------------------//

void Model::update( double delta )
{
	if( !mesh )
		return;

	if( !mesh->isLoaded() )
		return;

	if( !modelBuilt )
	{
		std::vector<RenderablePtr> renderables;
		mesh->appendRenderables( renderables );

		foreach( const RenderablePtr& rend, renderables )
		{
			MaterialPtr material = rend->getMaterial();

			if( material && material->isBlendingEnabled() )
				addRenderable( rend, RenderGroup::Transparency );
			else
				addRenderable( rend );
		}

		modelBuilt = true;
	}

	Geometry::update(delta);
}

//-----------------------------------//

} // end namespace