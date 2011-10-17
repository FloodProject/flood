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

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

DebugDrawer::DebugDrawer()
	: lines(nullptr)
	, triangles(nullptr)
{
	Allocator* alloc = AllocatorGetThis();
	
	MaterialHandle debug = MaterialCreate(alloc, "Debug");
	
	Material* mat = debug.Resolve();
	mat->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha);
	mat->setDepthTest(false);
	mat->setDepthWrite(false);

	GeometryBufferPtr linesVB = AllocateThis(GeometryBuffer);
	linesVB->setBufferAccess(BufferAccess::Write);
	linesVB->setBufferUsage(BufferUsage::Dynamic);

	lines = RenderableCreate(alloc);
	lines->setGeometryBuffer(linesVB);
	lines->setMaterial(debug);
	lines->setRenderLayer(RenderLayer::PostTransparency);

	GeometryBufferPtr trianglesVB = AllocateThis(GeometryBuffer);
	trianglesVB->setBufferAccess(BufferAccess::Write);
	trianglesVB->setBufferUsage(BufferUsage::Dynamic);

	triangles = RenderableCreate(alloc);
	triangles->setGeometryBuffer(trianglesVB);
	triangles->setMaterial(debug);
	triangles->setRenderLayer(RenderLayer::PostTransparency);
}

//-----------------------------------//

void DebugDrawer::reset()
{
	lines->getGeometryBuffer()->clear();
	triangles->getGeometryBuffer()->clear();
	renderables.clear();
}

//-----------------------------------//

void DebugDrawer::drawBox( const BoundingBox& box )
{
	RenderablePtr rend = DebugBuildBoundingBox(box);
	renderables.push_back(rend);
}

//-----------------------------------//

void DebugDrawer::drawRay( const Ray& ray, float length )
{
	RenderablePtr rend = DebugBuildRay(ray, length);
	renderables.push_back(rend);
}

//-----------------------------------//

void DebugDrawer::drawFrustum( const Frustum& frustum )
{
	RenderablePtr rend = DebugBuildFrustum(frustum);
	renderables.push_back(rend);
}

//-----------------------------------//

void DebugDrawer::drawIcon( const Vector3& pos )
{
}

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )		\
	pos.push_back( box.getCorner(a) );	\
	pos.push_back( box.getCorner(b) );	\
	pos.push_back( box.getCorner(c) );	\
	pos.push_back( box.getCorner(d) );

RenderablePtr DebugBuildBoundingBox( const BoundingBox& box )
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

	GeometryBufferPtr gb = Allocate(GeometryBuffer, AllocatorGetHeap());
	gb->set( VertexAttribute::Position, pos );
	gb->set( VertexAttribute::Color, colors );

	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "BoundingBoxDebug");
	
	Material* mat = materialHandle.Resolve();
	mat->setDepthCompare( DepthCompare::LessOrEqual );
	mat->setBackfaceCulling( false );
	//mat->setDepthRange( Vector2(0.1f, 0.9f) );

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Quads);
	renderable->setGeometryBuffer(gb);
	renderable->setMaterial(materialHandle);
	renderable->setPolygonMode( PolygonMode::Wireframe );

	return renderable;
}

//-----------------------------------//

RenderablePtr DebugBuildRay( const Ray& pickRay, float length )
{
	std::vector<Vector3> vertex;
	vertex.push_back( pickRay.origin );
	vertex.push_back( pickRay.getPoint(length) );

	std::vector<Vector3> colors( 2, Color::Red );

	GeometryBufferPtr gb = Allocate(GeometryBuffer, AllocatorGetHeap());
	gb->set( VertexAttribute::Position, vertex );
	gb->set( VertexAttribute::Color, colors );

	MaterialHandle material = MaterialCreate(AllocatorGetHeap(), "RayDebug");

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Lines);
	renderable->setGeometryBuffer(gb);
	renderable->setMaterial(material);
	
	return renderable;
}

//-----------------------------------//

RenderablePtr DebugBuildFrustum( const Frustum& box )
{
	GeometryBufferPtr gb = Allocate(GeometryBuffer, AllocatorGetHeap());

	MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "FrustumDebug");
	Material* material = materialHandle.Resolve();
	material->setBackfaceCulling( false );

	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Quads);
	renderable->setGeometryBuffer(gb);
	renderable->setMaterial(materialHandle);
	renderable->setPolygonMode( PolygonMode::Wireframe );

	DebugUpdateFrustum(renderable, box);
	return renderable;
}

//-----------------------------------//

#define ADD_BOX_FRUSTUM( a, b, c, d )	\
	pos.push_back( box.corners[a] );	\
	pos.push_back( box.corners[b] );	\
	pos.push_back( box.corners[c] );	\
	pos.push_back( box.corners[d] );

void DebugUpdateFrustum( const RenderablePtr& rend, const Frustum& box )
{
	std::vector<Vector3> pos;
	ADD_BOX_FRUSTUM( 0, 1, 3, 2 ) // Front
	ADD_BOX_FRUSTUM( 0, 1, 5, 4 ) // Top
	ADD_BOX_FRUSTUM( 4, 5, 7, 6 ) // Back
	ADD_BOX_FRUSTUM( 2, 3, 7, 6 ) // Bottom
	ADD_BOX_FRUSTUM( 0, 2, 6, 4 ) // Left
	ADD_BOX_FRUSTUM( 5, 1, 3, 7 ) // Right

	GeometryBufferPtr gb = rend->getGeometryBuffer();
	gb->set( VertexAttribute::Position, pos );

	std::vector<Vector3> colors( pos.size(), Color::White );
	gb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

NAMESPACE_ENGINE_END