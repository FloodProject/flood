/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Gizmo.h"

namespace vapor { namespace editor {

using namespace vapor::math;

//-----------------------------------//

const std::string& Gizmo::type = "Gizmo";

// Gizmo scaling factor.
const float S = 50.0;

//-----------------------------------//

Gizmo::Gizmo( const math::Vector3 midPoint )
	: midPoint( midPoint )
{	
	// Generate the gizmo lines.
	MaterialPtr mat( new Material( "Gizmo" ) );
	mat->setLineWidth( 2.0f );
	mat->setBackfaceCulling( false );
	mat->setProgram( "diffuse" );

	RenderablePtr rend( new Renderable( Primitive::Lines, generateLines() ) );
	rend->setMaterial( mat );
	
	addRenderable( rend );

	// Generate the gizmo cones.
	addRenderable( generateCones() );
}

//-----------------------------------//

render::VertexBufferPtr Gizmo::generateLines()
{
	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex position data
	std::vector< Vector3 > pos;

	// Vertex color data
	std::vector< Vector3 > colors;

	// X axis (red)
	pos.push_back( midPoint );
	pos.push_back( midPoint + Vector3::UnitX*S );
	//colors.push_back( Color( 0.6f, 0.0f, 0.0f ) );
	colors.push_back( Colors::Red );
	colors.push_back( Colors::Red );

	// Y axis (green)
	pos.push_back( midPoint + Vector3::UnitY*0.01f );
	pos.push_back( midPoint + Vector3::UnitY*S );
	//colors.push_back(Color( 0.0f, 0.4f, 0.0f ) );
	colors.push_back( Colors::Green );
	colors.push_back( Colors::Green );

	// Z axis (blue)
	pos.push_back( midPoint );
	pos.push_back( midPoint + Vector3::UnitZ*S );
	//colors.push_back( Color( 0.0f, 0.0f, 0.6f ) );
	colors.push_back( Colors::Blue );
	colors.push_back( Colors::Blue );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

#define CONE( tr, c1, c2 )					\
	foreach( const Vector3& v, cone ) {		\
		pos.push_back( v*tr ); }			\
	generateColors( SLICES, colors, c1, c2 );

const static byte SLICES = 10;

render::RenderablePtr Gizmo::generateCones()
{
	VertexBufferPtr vb( new VertexBuffer() );

	// Unit cone vertex data
	std::vector< Vector3 > cone;
	generateSolidCone( 5.0, 10.0, SLICES, cone );

	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	// We need to transform the unit cone so it is oriented correctly 
	// in the gizmo. A transformation matrix will take care of that.
	Matrix4x3 tr;

	tr = EulerAngles( 0.0, 0.0, -90.0 ).getOrientationMatrix();
	tr = tr*Matrix4x3::createTranslationMatrix( midPoint+Vector3::UnitX*S );
	CONE( tr, Colors::Red, Color( 0.6f, 0.0f, 0.0f ) );

	tr = Matrix4x3::createTranslationMatrix( midPoint+Vector3::UnitY*S );
	CONE( tr, Colors::Green, Color( 0.0f, 0.4f, 0.0f ) );

	tr = EulerAngles( 90.0, 0.0, 0.0 ).getOrientationMatrix();
	tr = tr*Matrix4x3::createTranslationMatrix( midPoint+Vector3::UnitZ*S );
	CONE( tr, Colors::Blue, Color( 0.0f, 0.0f, 0.6f ) );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	MaterialPtr mat( new Material( "ConeDiffuse", "diffuse" ) );
	mat->setBackfaceCulling( false );
	//mat->setDepthTest( false );

	RenderablePtr rend( new Renderable( Primitive::Triangles, vb, mat ) );
	return rend;
}

//-----------------------------------//

void Gizmo::generateColors( uint slices, std::vector<Vector3>& colors,
						   const Color& c1, const Color& c2 )
{
	// Darkens the color a bit.
	Vector3 baseColor = Vector3(c2);
	baseColor -= baseColor * 0.2f;
	
	for( uint i = 0; i < slices; i++ )
	{
		// Generate the base colors
		colors.push_back( baseColor);
		colors.push_back( baseColor );
		colors.push_back( baseColor );
	}

	for( uint i = 0; i < slices; i++ )
	{
		// Generate the top colors
		colors.push_back( c1 );
		colors.push_back( c1 );
		colors.push_back( c2 );
	}
}

//-----------------------------------//

void Gizmo::generateSolidCone( double base, double height, uint slices,
							  std::vector<Vector3>& pos )
{
	float r = 2*PI / slices;
	
	for( uint i = 0; i < slices; i++ )
	{
		// Generate the base
		pos.push_back( Vector3::Zero );
		pos.push_back( Vector3( cos(i*r), 0, sin(i*r) )*base );
		pos.push_back( Vector3( cos((i+1)*r), 0, sin((i+1)*r) )*base );
	}

	for( uint i = 0; i < slices; i++ )
	{
		// Generate the top of the cone
		pos.push_back( Vector3( cos(i*r), 0, sin(i*r) )*base );
		pos.push_back( Vector3( cos((i+1)*r), 0, sin((i+1)*r) )*base );
		pos.push_back( Vector3::UnitY*height );
	}
}

//-----------------------------------//

} } // end namespaces