/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoRotate.h"
#include "vapor/render/Cube.h"

namespace vapor { namespace editor {

//-----------------------------------//

const std::string& GizmoRotate::type = "GizmoRotate";

//-----------------------------------//

GizmoRotate::GizmoRotate( const NodePtr& node, const CameraWeakPtr& camera )
	: Gizmo( node, camera )
{ }

//-----------------------------------//

void GizmoRotate::buildGeometry()
{
	material->setLineWidth(3.0f);

	lines = generateCircles();
	addRenderable( new Renderable(Primitive::Lines, lines, material) );
}

//-----------------------------------//

void GizmoRotate::highlightAxis( GizmoAxis::Enum axis, bool highlight )
{

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

static const byte SLICES = 32;

VertexBufferPtr GizmoRotate::generateCircles()
{
	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	Matrix4x3 transform;

	// X axis
	generateCircle(pos, SLICES);
	generateColors(colors, X);

	// Y axis
	std::vector< Vector3 > posY;
	generateCircle(posY, SLICES);
	generateColors(colors, Y);

	transform = Matrix4x3::createOrientation( EulerAngles(0, 90, 0) );
	TransformVertices(pos, posY, transform);

	
	// Z axis
	std::vector< Vector3 > posZ;
	generateCircle(posZ, SLICES);
	generateColors(colors, Z);

	transform = Matrix4x3::createOrientation( EulerAngles(90, 0, 0) );
	TransformVertices(pos, posZ, transform);

	// Vertex buffer setup
	VertexBufferPtr vb = new VertexBuffer();

	assert( pos.size() == colors.size() );

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
	
	return vb;
}

//-----------------------------------//

void GizmoRotate::generateCircle(std::vector<Vector3>& pos, int slices)
{
	float phi = Math::PI / 2.0f;
	float delta = (Math::PI * 2.0f) / (float) slices;

	for( int i = 0; i <= slices; i++ )
	{
		float theta = i * delta;
		
		float x = sin(phi)*cos(theta);
		float y = sin(phi)*sin(theta);
		float z = cos(phi);

		Vector3 v(x, y, z);
		pos.push_back(v);

		if( i > 0 && i < slices )
			pos.push_back(v);	
	}
}

//-----------------------------------//

void GizmoRotate::generateColors( std::vector<Vector3>& colors, const Color& c )
{
	for( int i = 0; i <= SLICES; i++ )
	{
		colors.push_back(c);

		if( i > 0 && i < SLICES )
			colors.push_back(c);
	}
}

//-----------------------------------//

} } // end namespaces