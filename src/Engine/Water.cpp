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

const std::string& Water::type = "Water";

//-----------------------------------//

Water::Water( MaterialPtr material )
{
	quad = new Quad(100, 100);
	quad->setMaterial(material);

	addRenderable(quad, RenderGroup::Transparency);
}

//-----------------------------------//

} // end namespace
