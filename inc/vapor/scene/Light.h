/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/math/Color.h"

//-----------------------------------//

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
	~Light();

	void on();

	void off();

	bool isOn() const;

	void enableShadowCast();

	void disableShadowCast();

	bool castsShadow() const;

	void setType( LightType::Enum type );

	void setDiffuse( const math::Color& color );

	void setSpecular( const math::Color& color );

	void setEmissive( const math::Color& color );

	void setAmbient( const math::Color& color );

	void setSpotCutOffRadius( float radius );

	// Called once per frame to update the component.
	virtual void update( double delta );

	// Gets the type of this component. 
	virtual const std::string& getType() const;
  
public:

	// Light type and colors
	LightType::Enum lightType;

	math::Color diffuseColor;
	math::Color specularColor;
	math::Color emissiveColor;
	math::Color ambientColor;

	// Cutoff Radius for Spotlights
	float cutoffRadius;

protected:

	// Light state
	bool isLightOn;
	bool castsShadows;

	// Component type
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Light );

//-----------------------------------//

} } // end namespaces