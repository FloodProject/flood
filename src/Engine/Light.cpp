/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Light.h"
#include "vapor/render/Sphere.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(LightType)
	ENUM(Directional)
	ENUM(Point)
	ENUM(Spot)
END_ENUM()

BEGIN_CLASS_PARENT(Light, Component)
	FIELD_ENUM(Light, LightType, lightType)
	FIELD_PRIMITIVE(Light, Color, diffuseColor)
	FIELD_PRIMITIVE(Light, Color, specularColor)
	FIELD_PRIMITIVE(Light, Color, emissiveColor)
	FIELD_PRIMITIVE(Light, Color, ambientColor)
	FIELD_PRIMITIVE(Light, float, cutoffRadius)
	FIELD_PRIMITIVE(Light, bool, isLightOn)
	FIELD_PRIMITIVE(Light, bool, castsShadows)
END_CLASS()

//-----------------------------------//

Light::Light( LightType::Enum type )
	: lightType(type)
	, cutoffRadius(0)
{ }

//-----------------------------------//

void Light::update( double delta )
{
	if( !debugRenderable )
		debugRenderable = createDebugRenderable();
}

//-----------------------------------//

RenderablePtr Light::createDebugRenderable() const
{
	MaterialPtr matLight = new Material("LightMaterial");
	matLight->setDepthTest(false);

	SpherePtr sphere = new Sphere();
	sphere->setMaterial(matLight);

	return sphere;
}

//-----------------------------------//

} // end namespace