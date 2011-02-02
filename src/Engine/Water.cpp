/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Water.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Water, Geometry)
END_CLASS()

//-----------------------------------//

Water::Water()
{ }

//-----------------------------------//

Water::Water( MaterialPtr material )
{
	quad = new Quad(100, 100);
	quad->setRenderLayer(RenderLayer::Transparency);
	quad->setMaterial(material);

	addRenderable(quad);
}

//-----------------------------------//

} // end namespace
