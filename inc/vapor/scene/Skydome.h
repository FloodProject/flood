/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Color.h"
#include "vapor/render/Sphere.h"
#include "vapor/scene/Geometry.h"
#include "vapor/scene/Camera.h"

namespace vapor {

//-----------------------------------//

/**
 * Implements a skydome, a higher-quality alternative to skyboxes, using
 * half of a sphere in the background (can also be 3/4 or more if needed).
 * This component is tipically used to simulate a sky system, with moving
 * clouds, and a big texture representing the horizon, but this class only
 * takes care of the sphere generation and texture mapping. A proper sky
 * system can be implemented on top of this class if needed.
 */

class VAPOR_API Skydome : public Geometry
{
	DECLARE_CLASS_()

public:

	Skydome( const CameraPtr& camera );

	// SKY

	// Sets the sky to a fixed color.
	void setSkyColor( const Color& color );

	// Sets the sky to a linear color gradient.
	void setSkyLinearGradient( const Color& c1, const Color& c2 );

	// CLOUDS

	// Sets the texture of the clouds layer.
	void setClouds( const ImagePtr& clouds );

	// Enables or disables the clouds layer.
	ACESSOR( CloudsVisible, bool, showClouds );

	// CELESTIAL BODIES (Sun, Moon, Stars)

	// Sets the sun node.
	void setSunNode( const NodePtr& sun );
	
	// Gets the position of the sun.
	Vector3 getSunPosition();
	
	//void setStarsVisible( bool enable );

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

	// Keeps track of the current virtual game time.
	float currentTime;

	// Dome geometry that will be rendered as the sky.
	SpherePtr dome;
	
	// Sky color gradient.
	Color skyColorTop;
	Color skyColorBottom;
	float yMin, yMax;

	// Show clouds.
	bool showClouds;

	// Celestial bodies geometry.
	TransformPtr sun;

	// Current camera transform.
	CameraPtr camera;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Skydome );

//-----------------------------------//

} // end namespace