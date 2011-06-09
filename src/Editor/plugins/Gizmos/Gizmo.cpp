/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Gizmo.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Gizmo, Geometry)
REFLECT_CLASS_END()

const Color& Gizmo::X = Color::Red;
const Color& Gizmo::Y = Color::Green;
const Color& Gizmo::Z = Color::NavyBlue;

//-----------------------------------//

Gizmo::Gizmo( const EntityPtr& node, const CameraWeakPtr& camera )
	: nodeObject( node )
	, weakCamera( camera )
	, selectedAxis( GizmoAxis::None )
{	
	assert( node != nullptr );

	// Disable the depth testing so the gizmo can be seen behind objects.
	material = MaterialCreate(AllocatorGetHeap(), "GizmoMaterial");
	material.Resolve()->setDepthTest( false );
}

//-----------------------------------//

void Gizmo::updatePositionScale()
{
	const TransformPtr& transObject = nodeObject->getTransform();

	if( !transObject )
		return;

	const BoundingBox& boxObject = transObject->getWorldBoundingVolume();
	Vector3 centerObject = boxObject.getCenter();

	// Get the editor camera and its transform.
	CameraPtr camera = weakCamera;
	const EntityPtr& nodeCamera = camera->getEntity();
	const TransformPtr& transCamera = nodeCamera->getTransform();

	// Get the distance between the two points.
	const Vector3& posGizmo = boxObject.getCenter();
	const Vector3& posCamera = transCamera->getPosition();
	float distance = posCamera.distance(posGizmo);

	// Get the gizmo node and its transform.
	const EntityPtr& nodeGizmo = getEntity();
	const TransformPtr& transGizmo = nodeGizmo->getTransform();

	// Scale the gizmo relative to the distance.
	float scaleFactor = distance / 3.0f;
	transGizmo->setScale( scaleFactor );

	// Update the gizmo position to match the object.
	transGizmo->setPosition( centerObject );
}

//-----------------------------------//

void Gizmo::update( float delta )
{
	updatePositionScale();
	Geometry::update( delta );
}

//-----------------------------------//

void Gizmo::selectAxis( GizmoAxis::Enum axis )
{
	assert( axis != GizmoAxis::None );

	if( axis == selectedAxis )
		return;

	selectedAxis = axis;
	highlightAxis( selectedAxis, true );
}

//-----------------------------------//

void Gizmo::deselectAxis()
{
	if( selectedAxis == GizmoAxis::None )
		return;

	highlightAxis( selectedAxis, false );
	selectedAxis = GizmoAxis::None;
}

//-----------------------------------//

GizmoAxis::Enum Gizmo::getAxis(Color& c)
{
	if( c.nearEqual(X) )
		return GizmoAxis::X;

	else if( c.nearEqual(Y) )
		return GizmoAxis::Y;
	
	else if( c.nearEqual(Z) )
		return GizmoAxis::Z;

	return GizmoAxis::None;
}

//-----------------------------------//

Vector3 Gizmo::getAxisVector( GizmoAxis::Enum axis )
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

Color Gizmo::getAxisColor( GizmoAxis::Enum axis )
{
	assert( axis != GizmoAxis::None );

	if( axis == GizmoAxis::X )
		return X;
	
	else if( axis == GizmoAxis::Y )
		return Y;
	
	else if( axis == GizmoAxis::Z )
		return Z;

	return Color::Black;
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
	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());

	// Vertex position data
	std::vector< Vector3 > pos;

	// X axis
	pos.push_back( Vector3::Zero );
	pos.push_back( Vector3::UnitX / 2.0f );

	// Y axis
	pos.push_back( Vector3::UnitY*OriginOffset );
	pos.push_back( Vector3::UnitY / 2.0f );

	// Z axis
	pos.push_back( Vector3::Zero );
	pos.push_back( Vector3::UnitZ / 2.0f );

	// Vertex color data
	std::vector< Vector3 > colors;
	generateLinesColors(colors);

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

void Gizmo::generateLinesColors(std::vector<Vector3>& colors)
{
	// X axis
	colors.push_back( X );
	colors.push_back( X );

	// Y axis
	colors.push_back( Y );
	colors.push_back( Y );

	// Z axis
	colors.push_back( Z );
	colors.push_back( Z );
}

//-----------------------------------//

} } // end namespaces