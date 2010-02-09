/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Generates a sphere by subdividing a icosahedron.
 */

class Sphere : public render::Renderable
{
public:

	Sphere( );
	//virtual ~Sphere();

protected:

	// Subdivides a triangle into 4 sub-triangles.
	void subdivide(const math::Vector3& v1, 
		const math::Vector3& v2, const math::Vector3& v3);

	// Generates the sphere.
	void generateSphere();

	// Vertex data.
	std::vector<math::Vector3> pos;
	std::vector<math::Vector3> colors;
	std::vector<ushort> ind;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Sphere );

//-----------------------------------//

} } // end namespaces