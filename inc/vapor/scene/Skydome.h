/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/render/Sphere.h"

namespace vapor {

//-----------------------------------//

/**
 * Implements a skydome, an alternative to skyboxes, using a (semi) sphere
 * instead of a cube to simulate the horizon and sky. This component also
 * simulates a dynamic sky, with moving clouds.
 */

class VAPOR_API Skydome : public Geometry
{
	DECLARE_CLASS_()

public:

	Skydome();
	virtual ~Skydome();

	// SKY

	// Sets the sky to a fixed color.
	void setSkyColor( const Color& color );

	// Sets the sky to a linear color gradient.
	void setSkyLinearGradient( const Color& c1, const Color& c2 );

	// CLOUDS

	// Enables or disables the clouds layer.
	ACESSOR( CloudsVisible, bool, showClouds );

	// CELESTIAL BODIES (Sun, Moon, Stars)

	// Sets the sun node.
	void setSunEntity( const EntityPtr& sun );
	
	// Gets the position of the sun.
	Vector3 getSunPosition();

	// Gets called each cycle to update the component.
	void update( double delta );

protected:

	// Gets the sky color at the vertice at a given time of day.
	Color getSkyVertexColor( const Vector3& vertex );

	// Scales the Y vertex value into a [0,1] range.
	float scale( float number );

	// Generates the dome geometry.
	void generateDome();

	// Generates the sky bodies.
	void generateBodies();

	// Updates clouds.
	void updateClouds();

	// Transforms noise into clouds.
	float cloudsExpCurve(float v);

	// Field change callback.
	void onFieldChanged(const Field& field);

	// Keeps track of the current virtual game time.
	float currentTime;

	// Dome geometry that will be rendered as the sky.
	SpherePtr dome;
	
	// Sky color gradient.
	Color skyColorTop;
	Color skyColorBottom;
	float yMin, yMax;

	// Keeps track if sky follows the camera.
	bool followCamera;

	// Show clouds.
	bool showClouds;

	// Clouds cover.
	float cloudsCover;

	// Clouds cover.
	float cloudsSharpness;

	// Image of the clouds.
	ImagePtr clouds;

	int cloudsNoiseSeed;

	// Unprocessed clouds noise.
	std::vector<float> cloudsNoise;

	// Celestial bodies geometry.
	TransformPtr sun;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Skydome );

//-----------------------------------//

} // end namespace