/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Light.h"
#include "Render/Sphere.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(LightType)
	ENUM(Directional)
	ENUM(Point)
	ENUM(Spot)
END_ENUM()

BEGIN_CLASS_PARENT(Light, Component)
	FIELD_ENUM(LightType, lightType)
	FIELD_PRIMITIVE(Color, diffuseColor)
	FIELD_PRIMITIVE(Color, specularColor)
	FIELD_PRIMITIVE(Color, emissiveColor)
	FIELD_PRIMITIVE(Color, ambientColor)
	FIELD_PRIMITIVE(float, cutoffRadius)
	FIELD_PRIMITIVE(bool, isLightOn)
	FIELD_PRIMITIVE(bool, castsShadows)
END_CLASS()

//-----------------------------------//

Light::Light( LightType::Enum type )
	: lightType(type)
	, cutoffRadius(0)
{ }

//-----------------------------------//

void Light::update( float delta )
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