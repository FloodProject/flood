/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Sphere.h"
#include "vapor/math/Math.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//
// Icosahedron coordinates. Check out the following URLs more information:
// http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates
// http://www.geometrictools.com/Documentation/PlatonicSolids.pdf
// Code is also based from OpenGL Red Book example 2-4.

static const float t = (1 + math::sqrt(5.0f)) / 2;
static const float s = math::sqrt(1 + math::pow(t, 2));
static const float T = t / s;
static const float O = 1.0f / s;

//static const float X = .525731112119133606f;
//static const float Z = .850650808352039932f;

//static float vertices[12][3] =
//{
//	{-X, .0, Z}, {X, 0.0, Z}, {-X, .0, -Z}, {X, 0.0, -Z},
//	{0.0, Z, X}, {.0, Z, -X}, {0.0, -Z, X}, {.0, -Z, -X},
//	{Z, X, 0.0}, {-Z, X, .0}, {Z, -X, 0.0}, {-Z, -X, .0}
//};
//
//static byte indices[20][3] =
//{ 
//	{0,4,1},  {0,9,4},  {9,5,4},  {4,5,8},  {4,8,1},
//	{8,10,1}, {8,3,10}, {5,3,8},  {5,2,3},  {2,7,3},
//	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
//	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5},  {7,2,11}
//};

static float vertices[12][3] =
{
	{T, O, 0.0}, {-T, O, 0.0}, {T, -O, 0.0}, {-T, -O, 0.0},
	{O, 0.0, T}, {O, 0.0, -T}, {-O, 0.0, T}, {-O, 0.0, -T},
	{0.0, T, O}, {0.0, -T, O}, {0.0, T, -O}, {0.0, -T, -O}
};

static byte indices[20][3] =
{ 
	{0, 8, 4}, {0, 5, 10}, {2, 4, 9}, {2, 11, 5}, {1, 6, 8},
	{1, 10, 7}, {3, 9, 6}, {3, 7, 11}, {0, 10, 8}, {1, 8, 10},
	{2, 9, 11}, {3, 11, 9}, {4, 2, 0}, {5, 0, 2}, {6, 1, 3},
	{7, 3, 1}, {8, 6, 4}, {9, 4, 6}, {10, 5, 7}, {11, 7, 5},
};



//static float vertices[6][3] =
//{
//	{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
//	{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0},
//};
//
//static byte indices[20][3] =
//{ 
//	{4,0,2}, {4,2,1}, /*{4,1,3}, {4,3,0},*/
//	{5,2,0}, {5,1,2}, /*{5,3,1}, {5,0,3},*/
//};


//-----------------------------------//

Sphere::Sphere( ) : i( 0 )
{
	setPrimitiveType( Primitive::Triangles );
	generateSphere();

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
	ib->set( ind );

	pos.clear();
	colors.clear();
	ind.clear();
}

//-----------------------------------//

void Sphere::subdivide(const Vector3& v1, const Vector3& v2, const Vector3& v3, byte depth)
{
	if (depth == 0)
	{
		pos.push_back( v1 ); colors.push_back( Vector3::UnitX );
		pos.push_back( v2 ); colors.push_back( Vector3::UnitY );
		pos.push_back( v3 ); colors.push_back( Vector3::UnitZ );

		ind.push_back( i++ );
		ind.push_back( i++ );
		ind.push_back( i++ );
		
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

void Sphere::generateSphere( )
{
	vb = new VertexBuffer();
	ib = new IndexBuffer();

	foreach( const byte* i, indices )
	{
		Vector3 v1( vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2] );
		Vector3 v2( vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2] );
		Vector3 v3( vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2] );

		subdivide( v1, v2, v3, 1 );
	}
}

//-----------------------------------//

} } // end namespaces