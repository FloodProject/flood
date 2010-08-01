/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoScale.h"
#include "vapor/render/Cube.h"

namespace vapor { namespace editor {

//-----------------------------------//

const std::string& GizmoScale::type = "GizmoScale";

//-----------------------------------//

GizmoScale::GizmoScale( const NodePtr& node, const CameraWeakPtr& camera )
	: Gizmo( node, camera )
{ }

//-----------------------------------//

void GizmoScale::buildGeometry()
{
	lines = generateLines();
	addRenderable( new Renderable(Primitive::Lines, lines, material) );

	cubes = generateCubes();
	addRenderable( new Renderable(Primitive::Quads, cubes, material) );
}

//-----------------------------------//

void GizmoScale::highlightAxis( GizmoAxis::Enum axis, bool highlight )
{

}

//-----------------------------------//

VertexBufferPtr GizmoScale::generateCubes()
{
	static const byte SLICES = 10;

	VertexBufferPtr cube = Cube::buildGeometry(0.05f, 1);
	
	std::vector<Vector3>& cubePosition =
		cube->getAttribute(VertexAttribute::Position);

	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	// X axis
	foreach( const Vector3& v, cubePosition )
		pos.push_back( v + (Vector3::UnitX / 2.0f) );
	generateColors(colors, X);

	// Y axis
	foreach( const Vector3& v, cubePosition )
		pos.push_back( v + (Vector3::UnitY / 2.0f) );
	generateColors(colors, Y);
	
	// Z axis
	foreach( const Vector3& v, cubePosition )
		pos.push_back( v + (Vector3::UnitZ / 2.0f) );
	generateColors(colors, Z);

	// Vertex buffer setup
	VertexBufferPtr vb = new VertexBuffer();
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

void GizmoScale::generateColors( std::vector<Vector3>& colors, const Color& c )
{
	for( int i = 0; i < 24; i++ )
	{
		colors.push_back(c);
	}
}

//-----------------------------------//

} } // end namespaces