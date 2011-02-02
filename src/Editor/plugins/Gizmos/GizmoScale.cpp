/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoScale.h"
#include "render/Cube.h"

namespace vapor { namespace editor {

//-----------------------------------//

GizmoScale::GizmoScale( const EntityPtr& entity, const CameraWeakPtr& camera )
	: Gizmo(entity, camera)
{ }

//-----------------------------------//

void GizmoScale::buildGeometry()
{
	lines = generateLines();

	RenderablePtr renderable = new Renderable(PolygonType::Lines);
	renderable->setVertexBuffer(lines);
	renderable->setMaterial(material);

	addRenderable(renderable);

	cubes = generateCubes();

	renderable = new Renderable(PolygonType::Quads);
	renderable->setVertexBuffer(cubes);
	renderable->setMaterial(material);
	renderable->setRenderLayer(RenderLayer::PostTransparency);
	renderable->setRenderPriority(100);

	addRenderable(renderable);
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
	for( uint i = 0; i < cubePosition.size(); i++ )
	{
		const Vector3& v = cubePosition[i];
		pos.push_back( v + (Vector3::UnitX / 2.0f) );
	}
	generateColors(colors, X);

	// Y axis
	for( uint i = 0; i < cubePosition.size(); i++ )
	{
		const Vector3& v = cubePosition[i];
		pos.push_back( v + (Vector3::UnitY / 2.0f) );
	}
	generateColors(colors, Y);
	
	// Z axis
	for( uint i = 0; i < cubePosition.size(); i++ )
	{
		const Vector3& v = cubePosition[i];
		pos.push_back( v + (Vector3::UnitZ / 2.0f) );
	}
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