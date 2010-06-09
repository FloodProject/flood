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

const std::string& Light::type = "Light";

//-----------------------------------//

namespace LightType
{
	std::string toString(LightType::Enum e)
	{
		switch(e)
		{
		case LightType::Directional:
			return "Directional";
		case LightType::Spot:
			return "Spot";
		case LightType::Point:
			return "Point";
		}

		return "(unknown)";
	}
}

//-----------------------------------//

Light::Light( LightType::Enum type )
	: lightType(type), cutoffRadius(0.0f)
{
	//updateGeometry();
}

//-----------------------------------//

void Light::update( double delta )
{
}

//-----------------------------------//

void Light::serialize( Json::Value& value )
{
	value["lightType"] = LightType::toString(lightType);
	value["diffuseColor"] = toJson(diffuseColor);
	value["specularColor"] = toJson(specularColor);
	value["emissiveColor"] = toJson(emissiveColor);
	value["ambientColor"] = toJson(ambientColor);
	value["cutoffRadius"] = cutoffRadius;
	value["isLightOn"] = isLightOn;
	value["castsShadows"] = castsShadows;
}

//-----------------------------------//

} } // end namespaces