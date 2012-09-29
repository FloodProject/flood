/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Component.h"
#include "Core/Math/Color.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Lights can have different properties, depending on their purpose. 
 * Directional lights are modelled by a distant source of light that has 
 * a direction and no position, so it simulates light from a distant source.
 * Point lights just diffuse a uniform ammount of energy in all directions,
 * hence they have a position but no direction. Spot lights are modelled by
 * a cone, they have a direction and position and also a cut-off value.
 */

API_SCENE REFLECT_DECLARE_ENUM(LightType)

enum struct LightType
{
	Directional,
	Point,
	Spot
};

//-----------------------------------//

/**
 * Lights give life to a scene. Just add one and turn it on and there you go.
 * The light's position and direction should be set in the entity's Transform
 * component. Just rotate it to change the direction.
 */

API_SCENE REFLECT_DECLARE_CLASS(Light)

class API_ENGINE Light : public Component
{
	REFLECT_DECLARE_OBJECT(Light)

public:

	Light( LightType type = LightType::Point );

	// Gets/sets the type of this light source.
	ACCESSOR(LightType, LightType, lightType)

	// Gets/sets if this light source is active (powered on).
	ACCESSOR(Active, bool, isLightOn)

	// Gets/sets if this light source casts shadows.
	ACCESSOR(CastsShadows, bool, castsShadows)

	// Gets/sets the diffuse color of the light.
	ACCESSOR(DiffuseColor, const Color&, diffuseColor)
	
	// Gets/sets the specular color of the light.
	ACCESSOR(SpecularColor, const Color&, diffuseColor)

	// Gets/sets the emissive color of the light.
	ACCESSOR(EmissiveColor, const Color&, emissiveColor)

	// Gets/sets the ambient color of the light.
	ACCESSOR(AmbientColor, const Color&, ambientColor)

	// Gets/sets the cut-off radius of the light.
	ACCESSOR(CutOffRadius, float, cutoffRadius)

	// Called once per frame to update the component.
	virtual void update( float delta );

protected:

	// Creates the debug.renderable of this component.
	RenderBatchPtr createDebuRenderable() const;

	// Light type and colors
	LightType lightType;

	Color diffuseColor;
	Color specularColor;
	Color emissiveColor;
	Color ambientColor;

	// Cutoff Radius (for Spotlights).
	float cutoffRadius;

	// Light state
	bool isLightOn;
	bool castsShadows;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Light );

//-----------------------------------//

NAMESPACE_ENGINE_END