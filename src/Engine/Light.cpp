/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Light.h"

namespace vapor { namespace scene {

using namespace vapor::math;

//-----------------------------------//

const std::string& Light::type = "Light";

//-----------------------------------//

Light::Light( LightType::Enum type )
	: lightType( type )
{
	//updateGeometry();
}

//-----------------------------------//

void Light::update( double delta )
{
}

//-----------------------------------//

} } // end namespaces