/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Water.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Water, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Water::Water()
{
}

//-----------------------------------//

Water::Water( MaterialHandle material )
{
	quad = AllocateThis(Quad, 100, 100);
	quad->getRenderables()[0]->setRenderLayer(RenderLayer::Transparency);
	quad->getRenderables()[0]->setMaterial(material);

	addRenderable(getRenderables()[0]);
}

//-----------------------------------//

NAMESPACE_ENGINE_END
