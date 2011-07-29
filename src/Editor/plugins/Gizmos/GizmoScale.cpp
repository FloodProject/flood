/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "GizmoScale.h"
#include "Render/Cube.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

GizmoScale::GizmoScale( const EntityPtr& entity, const CameraWeakPtr& camera )
	: Gizmo(entity, camera)
{ }

//-----------------------------------//

void GizmoScale::buildGeometry()
{
	lines = generateLines();

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Lines);
	renderable->setVertexBuffer(lines);
	renderable->setMaterial(material);

	addRenderable(renderable);

	cubes = generateCubes();

	renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Quads);
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

	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	BuildCube(vb.get(), 0.05f, 1.0f);
	
	std::vector<Vector3>& cubePosition = vb->getAttribute(VertexAttribute::Position);

	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	// X axis
	for( size_t i = 0; i < cubePosition.size(); i++ )
	{
		const Vector3& v = cubePosition[i];
		pos.push_back( v + (Vector3::UnitX / 2.0f) );
	}
	generateColors(colors, X);

	// Y axis
	for( size_t i = 0; i < cubePosition.size(); i++ )
	{
		const Vector3& v = cubePosition[i];
		pos.push_back( v + (Vector3::UnitY / 2.0f) );
	}
	generateColors(colors, Y);
	
	// Z axis
	for( size_t i = 0; i < cubePosition.size(); i++ )
	{
		const Vector3& v = cubePosition[i];
		pos.push_back( v + (Vector3::UnitZ / 2.0f) );
	}
	generateColors(colors, Z);

	// Vertex buffer setup
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

NAMESPACE_EDITOR_END