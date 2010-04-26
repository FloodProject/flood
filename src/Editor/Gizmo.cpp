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

//-----------------------------------//

const std::string& Gizmo::type = "Gizmo";

// Gizmo scaling factor.
static const float S = 25.0f;

// Gizmo cone base color.
static const float BASE_FACTOR = 0.8f;

static const Color& X = Colors::Red;
static const Color& Y = Colors::Green;
static const Color& Z = Colors::Blue;

//-----------------------------------//

Gizmo::Gizmo( const NodePtr& node )
	: node( node ), selectedAxis( GizmoAxis::None )
{	
	assert( node != nullptr );

	const TransformPtr& tr = node->getTransform();
	const AABB& boundingVolume = tr->getWorldBoundingVolume();
	midPoint = boundingVolume.getCenter();

	// Generate the gizmo lines.
	MaterialPtr mat( new Material("Gizmo") );
	mat->setLineWidth( 2.0f );
	mat->setProgram( "diffuse" );
	mat->setDepthTest( false );

	lines = generateLines();
	addRenderable( new Renderable(Primitive::Lines, lines, mat) );

	cones = generateCones();
	addRenderable( new Renderable(Primitive::Triangles, cones, mat) );
}

//-----------------------------------//

void Gizmo::selectAxis( GizmoAxis::Enum axis )
{
	assert( axis != GizmoAxis::None );

	if( axis == selectedAxis )
		return;
	
	deselectAxis();
	selectedAxis = axis;
	
	std::vector<Vector3>& colors
		= lines->getAttribute( VertexAttribute::Color );

	assert( colors.size() == 3*2 );

	uint i = axis;
	colors[i] += Colors::White*0.5f;
	colors[i+1] += Colors::White*0.5f;

	lines->forceRebuild();
}

//-----------------------------------//

void Gizmo::deselectAxis()
{
	if( selectedAxis == GizmoAxis::None )
		return;

	std::vector<Vector3>& lines_colors
		= lines->getAttribute(VertexAttribute::Color);

	lines_colors.clear();

	// X axis (red)
	lines_colors.push_back( X );
	lines_colors.push_back( X );
	
	// Y axis (green)
	lines_colors.push_back( Y );
	lines_colors.push_back( Y );
	
	// Z axis (blue)
	lines_colors.push_back( Z );
	lines_colors.push_back( Z );

	lines->forceRebuild();

	selectedAxis = GizmoAxis::None;
}

//-----------------------------------//

GizmoAxis::Enum Gizmo::getAxis(Color& c)
{
	limit<float>(c.r, 0.0f, 1.0f);
	limit<float>(c.g, 0.0f, 1.0f);
	limit<float>(c.b, 0.0f, 1.0f);

	if( fcmp(c.r, X.r) || fcmp(c.r, X.r*BASE_FACTOR) )
		return GizmoAxis::X;
	
	else if( fcmp(c.g, Y.g) || fcmp(c.g, Y.g*BASE_FACTOR) )
		return GizmoAxis::Y;
	
	else if( fcmp(c.b, Z.b) || fcmp(c.b, Z.b*BASE_FACTOR) )
		return GizmoAxis::Z;

	return GizmoAxis::None;
}

//-----------------------------------//

Vector3 Gizmo::getUnitVector( GizmoAxis::Enum axis )
{
	assert( axis != GizmoAxis::None );

	if( axis == GizmoAxis::X )
		return Vector3::UnitX;
	
	else if( axis == GizmoAxis::Y )
		return Vector3::UnitY;
	
	else if( axis == GizmoAxis::Z )
		return Vector3::UnitZ;

	return Vector3::Zero;
}

//-----------------------------------//

bool Gizmo::isAnyAxisSelected() const
{
	return ( getAxisSelected() != GizmoAxis::None );
}

//-----------------------------------//

VertexBufferPtr Gizmo::generateLines()
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
	colors.push_back( X );
	colors.push_back( X );

	// Y axis (green)
	pos.push_back( midPoint + Vector3::UnitY*0.01f );
	pos.push_back( midPoint + Vector3::UnitY*S );
	//colors.push_back(Color( 0.0f, 0.4f, 0.0f ) );
	colors.push_back( Y );
	colors.push_back( Y );

	// Z axis (blue)
	pos.push_back( midPoint );
	pos.push_back( midPoint + Vector3::UnitZ*S );
	//colors.push_back( Color( 0.0f, 0.0f, 0.6f ) );
	colors.push_back( Z );
	colors.push_back( Z );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

#define CONE( tr )							\
	foreach( const Vector3& v, cone ) {		\
		pos.push_back( v*tr ); }

const static byte SLICES = 10;

VertexBufferPtr Gizmo::generateCones()
{
	VertexBufferPtr vb( new VertexBuffer() );

	// Unit cone vertex data
	std::vector< Vector3 > cone;
	generateSolidCone( 3.0, 10.0, SLICES, cone );

	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	// We need to transform the unit cone so it is oriented correctly 
	// in the gizmo. A transformation matrix will take care of that.
	Matrix4x3 tr;

	tr = EulerAngles( 0.0, 0.0, -90.0 ).getOrientationMatrix();
	tr = tr*Matrix4x3::createTranslationMatrix( midPoint+Vector3::UnitX*S );
	CONE( tr );
	generateColors( SLICES, colors, X, X /*Color(0.6f, 0.0f, 0.0f)*/ );


	tr = Matrix4x3::createTranslationMatrix( midPoint+Vector3::UnitY*S );
	CONE( tr );
	generateColors( SLICES, colors, Y, Y /*Color(0.0f, 0.4f, 0.0f)*/ );

	tr = EulerAngles( 90.0, 0.0, 0.0 ).getOrientationMatrix();
	tr = tr*Matrix4x3::createTranslationMatrix( midPoint+Vector3::UnitZ*S );
	CONE( tr );
	generateColors( SLICES, colors, Z, Z /*Color(0.0f, 0.0f, 0.6f)*/ );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

void Gizmo::generateColors( uint slices, std::vector<Vector3>& colors,
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
		pos.push_back( Vector3::UnitY*height );
		pos.push_back( Vector3( cos((i+1)*r), 0, sin((i+1)*r) )*base );
	}
}

//-----------------------------------//

} } // end namespaces