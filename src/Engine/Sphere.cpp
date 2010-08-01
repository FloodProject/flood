/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Sphere.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/Math.h"
#include "vapor/math/BoundingBox.h"

namespace vapor {

//-----------------------------------//

/**
 * Declare icosahedron base coordinates. We'll subdivide these
 * starting coordinates to get a more detailed sphere model.
 *
 * Check out the following URLs more information:
 * http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates
 * http://www.geometrictools.com/Documentation/PlatonicSolids.pdf
 * Code is also based from OpenGL Red Book example 2-4.
 */

//static const float t = (1 + sqrt(5.0f)) / 2;
//static const float s = sqrt(1 + pow(t, 2));

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
	{2, 11, 5}, {3, 7, 11}, {2, 9, 11}, {3, 11, 9}, {11, 7, 5},
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

Sphere::Sphere( bool fullSphere, byte numSubDiv, float dim )
{
	setPrimitiveType( Primitive::Triangles );
	
	VertexData position;
	buildGeometry( fullSphere, numSubDiv, position, dim );
	
	// Build Texture Coordinates.
	AABB box;
	
	foreach( const Vector3& v, position )
		box.add(v);

	Vector3 center = box.getCenter();

	std::vector<Vector3> texCoords;

	foreach( const Vector3& vert, position )
	{
		Vector3 d = vert-center;
		d.normalize();

		// Conveert to spherical coordinates.
		//float t = d.z / sqrt(d.x*d.x+d.y*d.y+d.z*d.z);
		//float delta = acos(t);
		//float phi = atan2(d.y, d.x);

		//float u = delta / Math::PI;
		//float v = phi / 2*Math::PI;
		float u = asinf(d.x) / Math::PI + 0.5f;
		float v = asinf(d.y) / Math::PI + 0.5f;

		texCoords.push_back( Vector2(u, v) );
	}

	vb->set( VertexAttribute::Position, position );
	vb->set( VertexAttribute::TexCoord0, texCoords );
}

//-----------------------------------//

void Sphere::subdivide(const Vector3& v1, const Vector3& v2, 
					   const Vector3& v3, byte depth, VertexData& pos)
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

	subdivide(v1, v12, v31, depth-1, pos);
	subdivide(v2, v23, v12, depth-1, pos);
	subdivide(v3, v31, v23, depth-1, pos);
	subdivide(v12, v23, v31, depth-1, pos);
}

//-----------------------------------//

void Sphere::buildGeometry( bool fullSphere, byte numSubDiv, 
							VertexData& pos, float dim)
{
	vb = new VertexBuffer();

	// Rotate the vertices, else the sphere is not properly aligned.
	Matrix4x3 rot = Matrix4x3::createRotation( EulerAngles(-60, 0, 0) );

	foreach( const byte* i, IcoDomeIndices )
	{
		Vector3 v1( IcoVertices[i[0]][0], IcoVertices[i[0]][2], IcoVertices[i[0]][1] );
		Vector3 v2( IcoVertices[i[1]][0], IcoVertices[i[1]][2], IcoVertices[i[1]][1] );
		Vector3 v3( IcoVertices[i[2]][0], IcoVertices[i[2]][2], IcoVertices[i[2]][1] );

		subdivide( v1*rot, v2*rot, v3*rot, numSubDiv, pos );
	}

	// If we don't want a full sphere, we return here.
	if( fullSphere )
	{
		// These indices are the bottom of the sphere.
		foreach( const byte* i, IcoSphereIndices )
		{
			Vector3 v1( IcoVertices[i[0]][0], IcoVertices[i[0]][2], IcoVertices[i[0]][1] );
			Vector3 v2( IcoVertices[i[1]][0], IcoVertices[i[1]][2], IcoVertices[i[1]][1] );
			Vector3 v3( IcoVertices[i[2]][0], IcoVertices[i[2]][2], IcoVertices[i[2]][1] );

			subdivide( v1*rot, v2*rot, v3*rot, numSubDiv, pos );
		}
	}

	// Scale all the vertices.
	foreach( Vector3& vec, pos )
	{
		vec *= dim;
	}
}

//-----------------------------------//

} // end namespace