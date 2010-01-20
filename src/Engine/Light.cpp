/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Light.h"

namespace vapor { namespace scene {

//using namespace vapor::math;

//-----------------------------------//

const std::string& Light::type = "Light";

//-----------------------------------//

Light::Light( LightType::Enum type )
	: lightType( type )
{
	//updateGeometry();
}

//-----------------------------------//

Light::~Light( )
{

}

//-----------------------------------//

void Light::on()
{

}

//-----------------------------------//

void Light::off()
{

}

//-----------------------------------//

bool Light::isOn() const
{
	return isLightOn;
}

//-----------------------------------//

void Light::enableShadowCast()
{
	
}

//-----------------------------------//

void Light::disableShadowCast()
{

}

//-----------------------------------//

bool Light::castsShadow() const
{
	return castsShadows;
}

//-----------------------------------//

void Light::setType( LightType::Enum type )
{

}

//-----------------------------------//

void Light::setDiffuse( const math::Color& color )
{

}

//-----------------------------------//

void Light::setSpecular( const math::Color& color )
{

}

//-----------------------------------//

void Light::setEmissive( const math::Color& color )
{

}

//-----------------------------------//

void Light::setAmbient( const math::Color& color )
{

}

//-----------------------------------//

void Light::setSpotCutOffRadius( float radius )
{

}

//-----------------------------------//

} } // end namespaces