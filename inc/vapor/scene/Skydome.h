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
#include "vapor/math/Color.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Implements a skydome, a higher-quality alternative to skyboxes, using
 * half of a sphere in the background (can also be 3/4 or more if needed).
 * This component is tipically used to simulate a sky system, with moving
 * clouds, and a big texture representing the horizon, but this class only
 * takes care of the sphere generation and texture mapping. A proper sky
 * system can be implemented on top of this class if needed.
 */

class Skydome : public scene::Geometry
{
public:

	Skydome( const render::MaterialPtr& mat );
	virtual ~Skydome();

	// Returns the name of this component.
	virtual const std::string& getType() const;

	// SKY

	// Sets the sky to a fixed color.
	void setSkyColor( const math::Color& color );

	// Sets the sky to a linear color gradient.
	void setSkyLinearGradient( const math::Color& c1, const math::Color& c2 );

	// CELESTIAL BODIES (Sun, Moon, Stars)
	
	// Celestial bodies.
	//void setStarsVisible( bool enable );

protected:

	// Gets the sky color at the vertice at a given time of day.
	math::Color getSkyVertexColor( const math::Vector3& vertex );

	// Scales the Y vertex value into a [0,1] range.
	float scale( float number );

	// Dome geometry that will be rendered as the sky.
	render::SpherePtr dome;
	math::Vector3 colorTop, colorBottom;
	float yMin, yMax;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Skydome );

//-----------------------------------//

} } // end namespaces