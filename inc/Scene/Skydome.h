/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"
#include "Render/Sphere.h"
#include "Resources/Image.h"

FWD_DECL_INTRUSIVE(Entity)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Implements a skydome, an alternative to skyboxes, using a (semi) sphere
 * instead of a cube to simulate the horizon and sky. This component also
 * simulates a dynamic sky, with moving clouds.
 */

REFLECT_DECLARE_CLASS(Skydome)

class API_SCENE Skydome : public Geometry
{
	REFLECT_DECLARE_OBJECT(Skydome)

public:

	Skydome();

	// Sets the base sky color.
	void setSkyColorBase( const Color& );

	// Sets the extra sky color.
	void setSkyColorExtra( const Color& );

	// Sets if sky colors are a linear gradient.
	void setSkyLinearGradient( bool useLinearGradient );

	// Enables or disables the clouds layer.
	GETTER( CloudsVisible, bool, showClouds );
	void setCloudsVisible( bool showClouds );

	// Sets the sun node.
	void setSunEntity( const EntityPtr& sun );
	
	// Gets the position of the sun.
	Vector3 getSunPosition();

	// Gets called each m to update the component.
	void update( float delta );

protected:

	// Gets the sky color at the vertice at a given time of day.
	Color getSkyVertexColor( const Vector3&, float minY, float maxY );

	// Generates the dome geometry.
	void generateDome();

	// Generates the sky bodies.
	void generateBodies();

	// Generates the sky colors.
	void generateColors();

	// Generates the clouds.
	void generateClouds();

	// Keeps track of the current virtual game time.
	float currentTime;

	// Dome geometry that will be rendered as the sky.
	SpherePtr dome;
	
	// Sky colors.
	Color skyBaseColor;
	Color skyExtraColor;

	// Generate linear gradient.
	bool useLinearGradient;

	// Keeps track if sky follows the camera.
	bool followCamera;

	// Show clouds.
	bool showClouds;

	// Keeps track if the geometry needs to be regenerated.
	bool needsRegeneration;

	// Clouds cover.
	float cloudsCover;

	// Clouds cover.
	float cloudsSharpness;

	// Image of the clouds.
	ImageHandle clouds;

	// Noise seed.
	int32 cloudsNoiseSeed;

	// Unprocessed clouds noise.
	std::vector<float> cloudsNoise;

	// Celestial bodies geometry.
	TransformPtr sun;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Skydome );

//-----------------------------------//

NAMESPACE_ENGINE_END