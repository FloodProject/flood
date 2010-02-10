/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Generates a sphere by subdividing a platonic solid, or what's known
 * as a geodesic sphere. It can also optionally generate a dome, which
 * has less faces than a full sphere and is suitable to simulate skies.
 * See http://en.wikipedia.org/wiki/Geodesic_dome for more details.
 */

class Sphere : public render::Renderable
{
public:

	Sphere( bool fullSphere = true, byte numSubDiv = 0 );

protected:

	// Subdivides a triangle into 4 sub-triangles.
	void subdivide( const math::Vector3& v1, const math::Vector3& v2,
		const math::Vector3& v3, byte depth );

	// Generates the sphere.
	void generateSphere( bool fullSphere, byte numSubDiv );

	// Vertex data.
	std::vector<math::Vector3> pos;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_CLASS( Sphere );

//-----------------------------------//

} } // end namespaces