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

// Gizmo cone base color.
static const float BASE_FACTOR = 0.3f;

static const Color& X = Color::Red;
static const Color& Y = Color::Green;
static const Color& Z = Color::Blue;

//-----------------------------------//

Gizmo::Gizmo( const NodePtr& node, const CameraWeakPtr& camera )
	: node( node )
	, weakCamera( camera )
	, selectedAxis( GizmoAxis::None )
{	
	assert( node != nullptr );
	assert( camera.lock() != nullptr );

	// Generate the gizmo lines.
	MaterialPtr mat( new Material("Gizmo") );
	mat->setProgram( "Diffuse" );
	mat->setLineWidth( 2.0f );
	mat->setDepthTest( false );
	mat->setDepthWrite( false );

	lines = generateLines();
	addRenderable( new Renderable(Primitive::Lines, lines, mat) );

	cones = generateCones();
	addRenderable( new Renderable(Primitive::Triangles, cones, mat) );
}

//-----------------------------------//

void Gizmo::update( double delta )
{
	// Get the objecty node and its transform.
	const NodePtr& nodeObject = node;
	const TransformPtr& transObject = nodeObject->getTransform();
	const AABB& boxObject = transObject->getWorldBoundingVolume();
	Vector3 centerObject = boxObject.getCenter();
	
	// Get the editor camera and its transform.
	CameraPtr camera = weakCamera.lock();
	const NodePtr& nodeCamera = camera->getNode();
	const TransformPtr& transCamera = nodeCamera->getTransform();

	// Get the distance between the two points.
	const Vector3& posGizmo = boxObject.getCenter();
	const Vector3& posCamera = transCamera->getPosition();
	float distance = posCamera.distance(posGizmo);

	// Get the gizmo node and its transform.
	const NodePtr& nodeGizmo = getNode();
	const TransformPtr& transGizmo = nodeGizmo->getTransform();

	// Scale the gizmo relative to the distance.
	float scaleFactor = distance / 5.0f;
	transGizmo->setScale( scaleFactor );
	debug( "distance: %f / scale: %f", distance, scaleFactor );

	// Update the gizmo position to match the object.
	transGizmo->setPosition( centerObject );
}

//-----------------------------------//

void Gizmo::selectAxis( GizmoAxis::Enum axis )
{
	assert( axis != GizmoAxis::None );

	if( axis == selectedAxis )
		return;
	
	deselectAxis();
	selectedAxis = axis;
	
	std::vector<Vector3>& colors =
		lines->getAttribute( VertexAttribute::Color );

	assert( colors.size() == 3*2 );

	// Re-color the axis for better perception
	uint i = axis;
	colors[i] += Color::White*0.5f;
	colors[i+1] += Color::White*0.5f;

	lines->forceRebuild();
}

//-----------------------------------//

void Gizmo::deselectAxis()
{
	if( selectedAxis == GizmoAxis::None )
		return;

	std::vector<Vector3>& linesColors =
		lines->getAttribute(VertexAttribute::Color);

	linesColors.clear();

	// X axis (red)
	linesColors.push_back( X );
	linesColors.push_back( X );
	
	// Y axis (green)
	linesColors.push_back( Y );
	linesColors.push_back( Y );
	
	// Z axis (blue)
	linesColors.push_back( Z );
	linesColors.push_back( Z );

	lines->forceRebuild();

	selectedAxis = GizmoAxis::None;
}

//-----------------------------------//

bool closeEnough(const Color& a, const Color& b)
{
	static const float epsilon = 0.05f;

	return (fabs(a.r - b.r) < epsilon)
		&& (fabs(a.g - b.g) < epsilon)
		&& (fabs(a.b - b.b) < epsilon);
}

//-----------------------------------//

GizmoAxis::Enum Gizmo::getAxis(Color& c)
{
	if( closeEnough(c, X) || closeEnough(c, X*BASE_FACTOR) )
		return GizmoAxis::X;
	
	else if( closeEnough(c, Y) || closeEnough(c, Y*BASE_FACTOR) )
		return GizmoAxis::Y;
	
	else if( closeEnough(c, Z) || closeEnough(c, Z*BASE_FACTOR) )
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
	GizmoAxis::Enum axis = getAxisSelected();

	return axis != GizmoAxis::None;
}

//-----------------------------------//

VertexBufferPtr Gizmo::generateLines()
{
	static const float OriginOffset = 0.01f;

	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex position data
	std::vector< Vector3 > pos;

	// Vertex color data
	std::vector< Vector3 > colors;

	// X axis
	pos.push_back( Vector3::Zero );
	pos.push_back( Vector3::UnitX );
	colors.push_back( X );
	colors.push_back( X );

	// Y axis
	pos.push_back( Vector3::UnitY*OriginOffset );
	pos.push_back( Vector3::UnitY );
	colors.push_back( Y );
	colors.push_back( Y );

	// Z axis
	pos.push_back( Vector3::Zero );
	pos.push_back( Vector3::UnitZ );
	colors.push_back( Z );
	colors.push_back( Z );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

#define TRANSFORM							\
	foreach( const Vector3& v, cone ) {		\
		pos.push_back( v*tr ); }

static const byte SLICES = 10;

VertexBufferPtr Gizmo::generateCones()
{
	VertexBufferPtr vb( new VertexBuffer() );

	// Unit cone vertex data
	std::vector< Vector3 > cone;
	generateSolidCone( 0.1f, 0.3f, SLICES, cone );

	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	// We need to transform the unit cone so it is oriented correctly 
	// in the gizmo. A transformation matrix will take care of that.
	Matrix4x3 tr;

	// X axis
	tr = EulerAngles(0, 0, -90).getOrientationMatrix();
	tr = tr*Matrix4x3::createTranslationMatrix( Vector3::UnitX );
	TRANSFORM;
	generateColors( SLICES, colors, X, X );

	// Y axis
	tr = Matrix4x3::createTranslationMatrix( Vector3::UnitY );
	TRANSFORM;
	generateColors( SLICES, colors, Y, Y );

	// Z axis
	tr = EulerAngles(90, 0, 0).getOrientationMatrix();
	tr = tr*Matrix4x3::createTranslationMatrix( Vector3::UnitZ );
	TRANSFORM;
	generateColors( SLICES, colors, Z, Z );

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