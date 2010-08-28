/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Geometry.h"
#include "vapor/scene/Node.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Geometry, Component)
END_CLASS()

//-----------------------------------//

Geometry::Geometry()
	: isDirty(true)
	, needsRenderCallback(false)
{ }

//-----------------------------------//

Geometry::Geometry( const RenderablePtr& rend )
	: isDirty(true)
	, needsRenderCallback(false)
{
	addRenderable( rend );
}

//-----------------------------------//

void Geometry::addRenderable( const RenderablePtr& rend, RenderStage::Enum group, 
							  uint priority )
{
	RenderableList& renderList = renderables[group];
	renderList.push_back( rend );
}

//-----------------------------------//

const RenderableList& Geometry::getRenderables( RenderStage::Enum group )
{ 
	return renderables[group]; 
}

//-----------------------------------//

void Geometry::markDirty()
{
	isDirty = true;
}

//-----------------------------------//

void Geometry::onRender()
{

}

//-----------------------------------//
 
void Geometry::appendRenderables( RenderQueue& queue, TransformPtr transform )
{
	if( !transform )
		return;
	
	const Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();
	
	foreach( const RenderableMapPair& pair, renderables )
	{
		foreach( const RenderablePtr& rend, pair.second )
		{
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = absoluteTransform;
			renderState.group = pair.first;
			renderState.priority = 0;

			if( needsRenderCallback )
				renderState.callback += fd::bind(&Geometry::onRender, this);

			queue.push_back( renderState );
		}
	}
}

//-----------------------------------//

void Geometry::update( double delta )
{
	if( !isDirty )
		return;

	boundingVolume.reset();

	// Update the bounding box to accomodate new geometry.
	foreach( const RenderablePtr& rend, renderables[RenderStage::Normal] )
	{
		const VertexBufferPtr& vb = rend->getVertexBuffer();
		
		if( !vb )
			continue;

		foreach( const Vector3& v, vb->getVertices() )
			boundingVolume.add( v );
	}

	const NodePtr& node = getNode();
	assert( node != nullptr );

	const TransformPtr& transform = node->getTransform();
	assert( transform != nullptr );

	transform->markBoundingVolumeDirty();
	isDirty = false;
}

//-----------------------------------//

} // end namespace