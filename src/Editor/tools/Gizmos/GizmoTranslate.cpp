/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoTranslate.h"

namespace vapor { namespace editor {

//-----------------------------------//

const std::string& GizmoTranslate::type = "GizmoTranslate";

// Gizmo cone base color.
static const float BASE_FACTOR = 0.3f;

//-----------------------------------//

GizmoTranslate::GizmoTranslate( const NodePtr& node, const CameraWeakPtr& camera )
	: Gizmo( node, camera )
{ }

//-----------------------------------//

void GizmoTranslate::buildGeometry()
{
	lines = generateLines();
	addRenderable( new Renderable(Primitive::Lines, lines, material) );

	cones = generateCones();
	addRenderable( new Renderable(Primitive::Triangles, cones, material) );
}

//-----------------------------------//

GizmoAxis::Enum GizmoTranslate::getAxis(Color& c)
{
	if( c.equals(X*BASE_FACTOR) )
		return GizmoAxis::X;
	
	else if( c.equals(Y*BASE_FACTOR) )
		return GizmoAxis::Y;
	
	else if( c.equals(Z*BASE_FACTOR) )
		return GizmoAxis::Z;

	return Gizmo::getAxis(c);
}

//-----------------------------------//

void GizmoTranslate::highlightAxis( GizmoAxis::Enum axis, bool highlight )
{
	std::vector<Vector3>& colors =
		lines->getAttribute( VertexAttribute::Color );

	assert( colors.size() == 6 ); // 2 vertices * 3 lines

	uint i = axis*2;

	if( highlight )
	{
		colors[i] += Color::White*0.5f;
		colors[i+1] += Color::White*0.5f;
	}
	else
	{
		colors[i] -= Color::White*0.5f;
		colors[i+1] -= Color::White*0.5f;
	}

	lines->forceRebuild();
}

//-----------------------------------//

static void TransformVertices(std::vector<Vector3>& pos,
					   std::vector<Vector3>& vs, Matrix4x3& transform)
{
	foreach( const Vector3& v, vs )
	{
		pos.push_back( v * transform );
	}
}

VertexBufferPtr GizmoTranslate::generateCones()
{
	static const byte SLICES = 10;

	VertexBufferPtr vb( new VertexBuffer() );

	// Unit cone vertex data
	std::vector< Vector3 > cone;
	generateSolidCone( 0.05f, 0.2f, SLICES, cone );

	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	// We need to transform the unit cone so it is oriented correctly 
	// in the gizmo. A transformation matrix will take care of that.
	Matrix4x3 transform;

	// X axis
	transform = Matrix4x3::createOrientation( EulerAngles(0, 0, -90) );
	transform *= Matrix4x3::createTranslation( Vector3::UnitX / 2.0f );
	TransformVertices(pos, cone, transform);
	generateColors( SLICES, colors, X, X );

	// Y axis
	transform = Matrix4x3::createTranslation( Vector3::UnitY / 2.0f );
	TransformVertices(pos, cone, transform);
	generateColors( SLICES, colors, Y, Y );

	// Z axis
	transform = Matrix4x3::createOrientation( EulerAngles(90, 0, 0) );
	transform *= Matrix4x3::createTranslation( Vector3::UnitZ / 2.0f );
	TransformVertices(pos, cone, transform);
	generateColors( SLICES, colors, Z, Z );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

void GizmoTranslate::generateColors( uint slices, std::vector<Vector3>& colors,
						    const Color& c1, const Color& c2 )
{
	// Darkens the color a bit.
	Vector3 baseColor = Vector3(c2);
	baseColor = baseColor * BASE_FACTOR;
	
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

void GizmoTranslate::generateSolidCone( double base, double height, uint slices,
							   std::vector<Vector3>& pos )
{
	float r = 2*Math::PI / slices;
	
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
		pos.push_back( Vector3::UnitY*height );
		pos.push_back( Vector3( cos((i+1)*r), 0, sin((i+1)*r) )*base );
	}
}

//-----------------------------------//

} } // end namespaces