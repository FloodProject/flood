/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/DebugGeometry.h"
#include "Render/Renderable.h"
#include "Scene/Geometry.h"
#include "Scene/Entity.h"
#include "Scene/Tags.h"

NAMESPACE_BEGIN

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )		\
	pos.push_back( box.getCorner(a) );	\
	pos.push_back( box.getCorner(b) );	\
	pos.push_back( box.getCorner(c) );	\
	pos.push_back( box.getCorner(d) );

RenderablePtr buildBoundingRenderable( const BoundingBox& box )
{
	std::vector<Vector3> pos;
	ADD_BOX_FACE( 0, 2, 3, 1 ) // Front
	ADD_BOX_FACE( 0, 1, 5, 4 ) // Bottom
	ADD_BOX_FACE( 4, 5, 7, 6 ) // Back
	ADD_BOX_FACE( 2, 6, 7, 3 ) // Top
	ADD_BOX_FACE( 0, 4, 6, 2 ) // Left
	ADD_BOX_FACE( 1, 3, 7, 5 ) // Right

	const int numColors = 6*4; // Faces*Vertices
	std::vector<Vector3> colors( numColors, Color::White );

	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "BoundingBoxDebug");
	Material* mat = materialHandle.Resolve();
	
	mat->setDepthCompare( DepthCompare::LessOrEqual );
	mat->setBackfaceCulling( false );

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Quads);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(materialHandle);
	renderable->setPolygonMode( PolygonMode::Wireframe );

	return renderable;
}

//-----------------------------------//

EntityPtr buildRay( const Ray& pickRay, const Vector3& outFar )
{
	std::vector<Vector3> vertex;
	vertex.push_back( pickRay.origin );
	vertex.push_back( outFar );

	std::vector<Vector3> colors( 2, Color::Red );

	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );

	MaterialHandle material = MaterialCreate(AllocatorGetHeap(), "RayDebug");

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Lines);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(material);
	
	GeometryPtr geometry = Allocate(Geometry, AllocatorGetHeap(), renderable);
	
	EntityPtr line( EntityCreate(AllocatorGetHeap()) );
	line->setName("Line");
	line->setTag( Tags::NonPickable, true );
	line->addTransform();
	line->addComponent( geometry );
	
	return line;
}

//-----------------------------------//

RenderablePtr buildFrustum( const Frustum& box )
{
	const int numColors = 6*4; // Faces*Vertices
	std::vector<Vector3> colors( numColors, Color::White );

	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());
	vb->set( VertexAttribute::Color, colors );

	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "FrustumDebug");
	Material* material = materialHandle.Resolve();
	material->setBackfaceCulling( false );

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Quads);
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(materialHandle);
	renderable->setPolygonMode( PolygonMode::Wireframe );

	updateDebugFrustum(renderable, box);
	return renderable;
}

//-----------------------------------//

#define ADD_BOX_FRUSTUM( a, b, c, d )	\
	pos.push_back( box.corners[a] );	\
	pos.push_back( box.corners[b] );	\
	pos.push_back( box.corners[c] );	\
	pos.push_back( box.corners[d] );

void updateDebugFrustum( const RenderablePtr& rend, const Frustum& box )
{
	std::vector<Vector3> pos;
	ADD_BOX_FRUSTUM( 0, 1, 3, 2 ) // Front
	ADD_BOX_FRUSTUM( 0, 1, 5, 4 ) // Top
	ADD_BOX_FRUSTUM( 4, 5, 7, 6 ) // Back
	ADD_BOX_FRUSTUM( 2, 3, 7, 6 ) // Bottom
	ADD_BOX_FRUSTUM( 0, 2, 6, 4 ) // Left
	ADD_BOX_FRUSTUM( 5, 1, 3, 7 ) // Right

	VertexBufferPtr vb = rend->getVertexBuffer();
	vb->set( VertexAttribute::Position, pos );
}

//-----------------------------------//

NAMESPACE_END