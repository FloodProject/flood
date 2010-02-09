/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Sphere.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//
// Icosahedron coordinates. Check out the following URL more information
// http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates
// Based from an OpenGL Red Book example 2-4.

static const float X = .525731112119133606f;
static const float Z = .850650808352039932f;

static float vertices[12][3] =
{
	{-X, .0, Z}, {X, 0.0, Z}, {-X, .0, -Z}, {X, 0.0, -Z},
	{0.0, Z, X}, {.0, Z, -X}, {0.0, -Z, X}, {.0, -Z, -X},
	{Z, X, 0.0}, {-Z, X, .0}, {Z, -X, 0.0}, {-Z, -X, .0}
};

static byte indices[20][3] =
{ 
	{0,4,1},  {0,9,4},  {9,5,4},  {4,5,8},  {4,8,1},
	{8,10,1}, {8,3,10}, {5,3,8},  {5,2,3},  {2,7,3},
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5},  {7,2,11}
};

//-----------------------------------//

Sphere::Sphere( )
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

#define ADD_FACE( a, b, c )			\
	ind.push_back( a );				\
	ind.push_back( b );				\
	ind.push_back( c );

void Sphere::subdivide(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	static ushort i = 0;

	Vector3 v12 = (v1+v2).normalize();
	Vector3 v23 = (v2+v3).normalize();
	Vector3 v31 = (v3+v1).normalize();

	pos.push_back( v1 ); colors.push_back( Vector3::UnitX );
	pos.push_back( v2 ); colors.push_back( Vector3::UnitY );
	pos.push_back( v3 ); colors.push_back( Vector3::UnitZ );
	pos.push_back( v12 ); colors.push_back( Vector3::UnitX );
	pos.push_back( v23 ); colors.push_back( Vector3::UnitY );
	pos.push_back( v31 ); colors.push_back( Vector3::UnitZ );

	ADD_FACE( i,   i+3, i+5 );
	ADD_FACE( i+1, i+4, i+3 );
	ADD_FACE( i+2, i+5, i+4 );
	ADD_FACE( i+3, i+4, i+5 );

	i += 6;
}

//-----------------------------------//

void Sphere::generateSphere( )
{
	vb = new VertexBuffer();
	ib = new IndexBuffer();

	//foreach( const float* v, vertices )
	//{
	//	pos.push_back( Vector3( v[0], v[1], v[2] ) );
	//	colors.push_back( Vector3( 1.0f, 1.0f, 1.0f ) );
	//}

	foreach( const byte* i, indices )
	{
		Vector3 v1( vertices[i[0]][0], vertices[i[0]][1], vertices[i[0]][2] );
		Vector3 v2( vertices[i[1]][0], vertices[i[1]][1], vertices[i[1]][2] );
		Vector3 v3( vertices[i[2]][0], vertices[i[2]][1], vertices[i[2]][2] );

		subdivide( v1, v2, v3 );
	}
}

//-----------------------------------//

} } // end namespaces