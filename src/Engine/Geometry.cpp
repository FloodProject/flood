/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Geometry.h"
#include "vapor/scene/Node.h"

namespace vapor { namespace scene {

using namespace vapor::render;
using namespace vapor::math;

//-----------------------------------//

const std::string& Geometry::type = "Geometry";

//-----------------------------------//

Geometry::Geometry() 
	: isDirty( true ), drawBoundingBox( false )
{
	buildBoundingRenderable();
}

//-----------------------------------//

Geometry::Geometry( RenderablePtr rend )
	: isDirty( true ), drawBoundingBox( false )
{
	addRenderable( rend );
	buildBoundingRenderable();
}

//-----------------------------------//

Geometry::~Geometry()
{

}

//-----------------------------------//

void Geometry::buildBoundingRenderable()
{
	VertexBufferPtr vb( new VertexBuffer() );
	MaterialPtr mat( new Material( "BoundBox", "diffuse" ) );
	mat->setLineWidth( 1.0f );
	mat->setLineSmoothing( true );
	bbox.reset( new Renderable( Primitive::Quads, vb, mat ) );
	bbox->setRenderMode( RenderMode::Wireframe );
}

//-----------------------------------//

void Geometry::markDirty()
{
	isDirty = true;
}

//-----------------------------------//

void Geometry::addRenderable( RenderablePtr rend, 
							 RenderGroup::Enum group, 
							 uint priority )
{
	renderables[group].push_back( rend );
}

//-----------------------------------//

const std::vector< render::RenderablePtr >& 
Geometry::getRenderables( RenderGroup::Enum group )
{ 
	return renderables[group]; 
}

//-----------------------------------//
 
void Geometry::appendRenderables( render::RenderQueue& queue, TransformPtr transform )
{
	//TransformPtr transform = getNode()->getTransform();
	
	// The node should have a transform component.
	assert( transform != nullptr );

	if( !transform ) return;
	
	const math::Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();
	
	foreach( const rendPair& pair, renderables )
	{
		foreach( RenderablePtr rend, pair.second )
		{
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = absoluteTransform;
			renderState.group = pair.first;
			renderState.priority = 0;

			queue.push_back( renderState );
		}
	}

	if( drawBoundingBox )
	{
		RenderState renderState;
		
		renderState.renderable = bbox;
		renderState.modelMatrix = absoluteTransform;
		renderState.group = RenderGroup::Normal;
		renderState.priority = 0;

		queue.push_back( renderState );
	}
}

//-----------------------------------//

const math::AABB& Geometry::getBoundingVolume() const
{
	return boundingVolume;
}

//-----------------------------------//

static const float EXTRA_SPACE = 1.01f;

#define ADD_BOX_FACE( a, b, c, d )								\
	v.push_back( boundingVolume.getCorner( a ) * EXTRA_SPACE );	\
	v.push_back( boundingVolume.getCorner( b ) * EXTRA_SPACE );	\
	v.push_back( boundingVolume.getCorner( c ) * EXTRA_SPACE );	\
	v.push_back( boundingVolume.getCorner( d ) * EXTRA_SPACE );

void Geometry::update( float delta )
{
	if( !isDirty ) return;

	boundingVolume.reset();

	// Update the bounding box to accomodate new geometry.
	foreach( RenderablePtr rend, renderables[RenderGroup::Normal] )
	{
		std::vector<math::Vector3> vertices = rend->getVertexBuffer()->getVertices();
		
		foreach( const Vector3& v, vertices )
			boundingVolume.add( v );
	}

	VertexBufferPtr vb = bbox->getVertexBuffer();

	std::vector< Vector3 > v;
	
	ADD_BOX_FACE( 0, 1, 3, 2 )
	ADD_BOX_FACE( 0, 1, 5, 4 )
	ADD_BOX_FACE( 4, 5, 7, 6 )
	ADD_BOX_FACE( 2, 3, 7, 6 )
	ADD_BOX_FACE( 0, 4, 6, 2 )
	ADD_BOX_FACE( 1, 5, 7, 3 )

	std::vector< Vector3 > c( 24, Vector3( 1.0f, 1.0f, 0.0f ) );

	vb->set( VertexAttribute::Vertex, v );
	vb->set( VertexAttribute::Color, c );

	isDirty = false;
}

//-----------------------------------//

const math::AABB& Geometry::getBoundingBox() const
{
	return boundingVolume;
}

//-----------------------------------//

void Geometry::setBoundingBoxVisible( bool visible )
{
	drawBoundingBox = visible;
}

//-----------------------------------//

const std::string Geometry::save(int indent)
{
	return "";
}

//-----------------------------------//

const std::string& Geometry::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces