/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Sphere.h"
#include "vapor/math/Math.h"
#include "vapor/math/EulerAngles.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//
// Declare icosahedron base coordinates. We'll subdivide these
// starting coordinates to get a more detailed sphere model.
// Check out the following URLs more information:
// http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates
// http://www.geometrictools.com/Documentation/PlatonicSolids.pdf
// Code is also based from OpenGL Red Book example 2-4.

//static const float t = (1 + math::sqrt(5.0f)) / 2;
//static const float s = math::sqrt(1 + math::pow(t, 2));

static const float T = .850650808352039932f; // t / s;
static const float O = .525731112119133606f; // 1.0f / s;

static const float IcoVertices[][3] =
{
	{T, O, 0.0}, {-T, O, 0.0}, {T, -O, 0.0}, {-T, -O, 0.0},
	{O, 0.0, T}, {O, 0.0, -T}, {-O, 0.0, T}, {-O, 0.0, -T},
	{0.0, T, O}, {0.0, -T, O}, {0.0, T, -O}, {0.0, -T, -O}
};

static const byte IcoDomeIndices[][3] =
{
	{0, 8, 4}, {0, 5, 10}, {2, 4, 9}, /*{2, 11, 5},*/ {1, 6, 8},
	{1, 10, 7}, {3, 9, 6}, /*{3, 7, 11},*/ {0, 10, 8}, {1, 8, 10},
	/*{2, 9, 11},*/ /*{3, 11, 9},*/ {4, 2, 0}, {5, 0, 2}, {6, 1, 3},
	{7, 3, 1}, {8, 6, 4}, {9, 4, 6}, {10, 5, 7}, /*{11, 7, 5},*/
};

static const byte IcoSphereIndices[][3] =
{
	{2, 11, 5}, {3, 7, 11}, {2, 9, 11}, {3, 11, 9},
	{11, 7, 5},
};

//-----------------------------------//
// Octahedron coordinates.

static const float OctaVertices[][3] =
{
	{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0},
};

static const byte OctaDomeIndices[][3] =
{
	{4,0,2}, {4,2,1}, {5,2,0}, {5,1,2},
};

static const byte OctaSphereIndices[][3] =
{
	{4,1,3}, {4,3,0}, {5,3,1}, {5,0,3},
};

//-----------------------------------//

Sphere::Sphere( bool fullSphere, byte numSubDiv )
{
	setPrimitiveType( Primitive::Triangles );
	setPolygonMode( PolygonMode::Wireframe );
	
	generateSphere( fullSphere, numSubDiv );
	vb->set( VertexAttribute::Position, pos );
	pos.clear();
}

//-----------------------------------//

void Sphere::subdivide(const Vector3& v1, const Vector3& v2, const Vector3& v3, byte depth)
{
	if (depth == 0)
	{
		pos.push_back( v1 );
		pos.push_back( v2 );
		pos.push_back( v3 );
		
		return;
	}

	Vector3 v12 = (v1+v2).normalize();
	Vector3 v23 = (v2+v3).normalize();
	Vector3 v31 = (v3+v1).normalize();

	subdivide(v1, v12, v31, depth-1);
	subdivide(v2, v23, v12, depth-1);
	subdivide(v3, v31, v23, depth-1);
	subdivide(v12, v23, v31, depth-1);
}

//-----------------------------------//

void Sphere::generateSphere( bool fullSphere, byte numSubDiv )
{
	vb = new VertexBuffer();

	Matrix4x3 rot( EulerAngles( -30.0f, 0.0f, 0.0f ).getOrientationMatrix() );

	foreach( const byte* i, IcoDomeIndices )
	{
		Vector3 v1( IcoVertices[i[0]][0], IcoVertices[i[0]][1], IcoVertices[i[0]][2] );
		Vector3 v2( IcoVertices[i[1]][0], IcoVertices[i[1]][1], IcoVertices[i[1]][2] );
		Vector3 v3( IcoVertices[i[2]][0], IcoVertices[i[2]][1], IcoVertices[i[2]][2] );

		subdivide( v1*rot, v2*rot, v3*rot, numSubDiv );
	}

	if( !fullSphere ) return;

	foreach( const byte* i, IcoSphereIndices )
	{
		Vector3 v1( IcoVertices[i[0]][0], IcoVertices[i[0]][1], IcoVertices[i[0]][2] );
		Vector3 v2( IcoVertices[i[1]][0], IcoVertices[i[1]][1], IcoVertices[i[1]][2] );
		Vector3 v3( IcoVertices[i[2]][0], IcoVertices[i[2]][1], IcoVertices[i[2]][2] );

		subdivide( v1*rot, v2*rot, v3*rot, numSubDiv );
	}
}

//-----------------------------------//

} } // end namespaces