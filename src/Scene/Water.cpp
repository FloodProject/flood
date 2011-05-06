/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Water.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Water, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Water::Water()
{ }

//-----------------------------------//

Water::Water( MaterialHandle material )
{
	quad = new Quad(100, 100);
	quad->setRenderLayer(RenderLayer::Transparency);
	quad->setMaterial(material);

	addRenderable(quad);
}

//-----------------------------------//

} // end namespace
