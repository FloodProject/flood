/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/math/Color.h"

namespace vapor {

//-----------------------------------//

/**
 * Lights can have different properties, depending on their purpose. 
 * Directional lights are modelled by a distant source of light that has 
 * a direction and no position, so it simulates light from a distant source.
 * Point lights just diffuse a uniform ammount of energy in all directions,
 * hence they have a position but no direction. Spot lights are modelled by
 * a cone, they have a direction and position and also a cut-off value.
 */

struct LightType
{
	enum Enum
	{
		Directional,
		Point,
		Spot
	};

	DECLARE_ENUM()
};

//-----------------------------------//

/**
 * Lights give life to a scene. Just add one and turn it on and there you go.
 * The light's position and direction should be set in the entity's Transform
 * component. Just rotate it to change the direction.
 */

class VAPOR_API Light : public Component
{
	DECLARE_CLASS_()

public:

	Light( LightType::Enum type = LightType::Point );

	// Gets/sets the type of this light source.
	ACESSOR(LightType, LightType::Enum, lightType)

	// Gets/sets if this light source is active (powered on).
	ACESSOR(Active, bool, isLightOn)

	// Gets/sets if this light source casts shadows.
	ACESSOR(CastsShadows, bool, castsShadows)

	// Gets/sets the diffuse color of the light.
	ACESSOR(DiffuseColor, const Color&, diffuseColor)
	
	// Gets/sets the specular color of the light.
	ACESSOR(SpecularColor, const Color&, diffuseColor)

	// Gets/sets the emissive color of the light.
	ACESSOR(EmissiveColor, const Color&, emissiveColor)

	// Gets/sets the ambient color of the light.
	ACESSOR(AmbientColor, const Color&, ambientColor)

	// Gets/sets the cut-off radius of the light.
	ACESSOR(CutOffRadius, float, cutoffRadius)

	// Called once per frame to update the component.
	virtual void update( float delta );

protected:

	// Creates the debug renderable of this component.
	RenderablePtr createDebugRenderable() const;

	// Light type and colors
	LightType::Enum lightType;

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

TYPEDEF_SHARED_POINTER_FROM_TYPE( Light );

//-----------------------------------//

} // end namespace