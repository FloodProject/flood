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

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Lights can have different properties, depending on their purpose. 
 * Directional lights are modelled by a distant source of light that has 
 * a direction and no position, so it simulates light from a distant source.
 * Point lights just diffuse a uniform ammount of energy in all directions,
 * hence they have a position but no direction. Spot lights are modelled by
 * a cone, they have a direction and position and also a cut-off value.
 */

namespace LightType
{
	enum Enum
	{
		Directional,
		Point,
		Spot
	};

	static std::string toString(LightType::Enum);
}

//-----------------------------------//

/**
 * Lights give life to a scene. Just add one and turn it on and there you go.
 * The light's position and direction should be set in the entity's Transform
 * component. Just rotate it to change the direction.
 */

class Light : public Component
{
public:

	Light( LightType::Enum type );

	// Gets/sets the type of this light source.
	IMPLEMENT_ACESSOR(LightType, LightType::Enum, lightType)

	// Gets/sets if this light source is active (powered on).
	IMPLEMENT_ACESSOR(Active, bool, isLightOn)

	// Gets/sets if this light source casts shadows.
	IMPLEMENT_ACESSOR(CastsShadows, bool, castsShadows)

	// Gets/sets the diffuse color of the light.
	IMPLEMENT_ACESSOR(DiffuseColor, const math::Color&, diffuseColor)
	
	// Gets/sets the specular color of the light.
	IMPLEMENT_ACESSOR(SpecularColor, const math::Color&, diffuseColor)

	// Gets/sets the emissive color of the light.
	IMPLEMENT_ACESSOR(EmissiveColor, const math::Color&, emissiveColor)

	// Gets/sets the ambient color of the light.
	IMPLEMENT_ACESSOR(AmbientColor, const math::Color&, ambientColor)

	// Gets/sets the cut-off radius of the light.
	IMPLEMENT_ACESSOR(CutOffRadius, float, cutoffRadius)

	// Gets the type of this component. 
	IMPLEMENT_GETTER(Type, const std::string&, Light::type)

	// Called once per frame to update the component.
	virtual void update( double delta );

	DECLARE_SERIALIZABLE();
  
protected:

	// Light type and colors
	LightType::Enum lightType;

	math::Color diffuseColor;
	math::Color specularColor;
	math::Color emissiveColor;
	math::Color ambientColor;

	// Cutoff Radius for Spotlights
	float cutoffRadius;

	// Light state
	bool isLightOn;
	bool castsShadows;

	// Component type
	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Light );

//-----------------------------------//

} } // end namespaces