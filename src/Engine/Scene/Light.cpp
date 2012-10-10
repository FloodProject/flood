/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Scene/Light.h"
#include "Engine/Geometry/Sphere.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(LightType)
	ENUM(Directional)
	ENUM(Point)
	ENUM(Spot)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Light, Component)
	FIELD_ENUM(4, LightType, lightType)
	FIELD_PRIMITIVE(5, Color, diffuseColor)
	FIELD_PRIMITIVE(6, Color, specularColor)
	FIELD_PRIMITIVE(7, Color, emissiveColor)
	FIELD_PRIMITIVE(8, Color, ambientColor)
	FIELD_PRIMITIVE(9, float, cutoffRadius)
	FIELD_PRIMITIVE(10, bool, isLightOn)
	FIELD_PRIMITIVE(11, bool, castsShadows)
REFLECT_CLASS_END()

//-----------------------------------//

Light::Light( LightType type )
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

RenderBatchPtr Light::createDebugRenderable() const
{
	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "LightMaterial");

	Material* material = materialHandle.Resolve();
	material->setDepthTest(false);

	SpherePtr sphere = new Sphere();
	sphere->setMaterial(materialHandle);

	return sphere;
}

//-----------------------------------//

NAMESPACE_ENGINE_END