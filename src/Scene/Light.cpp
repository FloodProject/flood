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

REFLECT_ENUM(LightType)
	ENUM(Directional)
	ENUM(Point)
	ENUM(Spot)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Light, Component)
	FIELD_ENUM(LightType, lightType)
	FIELD_PRIMITIVE(Color, diffuseColor)
	FIELD_PRIMITIVE(Color, specularColor)
	FIELD_PRIMITIVE(Color, emissiveColor)
	FIELD_PRIMITIVE(Color, ambientColor)
	FIELD_PRIMITIVE(float, cutoffRadius)
	FIELD_PRIMITIVE(bool, isLightOn)
	FIELD_PRIMITIVE(bool, castsShadows)
REFLECT_CLASS_END()

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
	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "LightMaterial");

	Material* material = materialHandle.Resolve();
	material->setDepthTest(false);

	SpherePtr sphere = new Sphere();
	sphere->setMaterial(materialHandle);

	return sphere;
}

//-----------------------------------//

} // end namespace