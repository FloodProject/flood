/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Geometry.h"
#include "vapor/scene/Entity.h"

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

RenderableList Geometry::getRenderables( RenderStage::Enum group )
{
	RenderableList rends;

	RenderableMap::const_iterator it;
	for( it = renderables.cbegin(); it != renderables.cend(); it++ )
	{
		const RenderableList& groupRends = it->second;
		
		for( uint i = 0; i < groupRends.size(); i++ )
			rends.push_back(groupRends[i]);
	}

	//return renderables[group];
	return rends;
}

//-----------------------------------//
 
void Geometry::appendRenderables( RenderQueue& queue, TransformPtr transform )
{
	if( !transform )
		return;
	
	const Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();
	
	RenderableMap::const_iterator it;
	for( it = renderables.cbegin(); it != renderables.cend(); it++ )
	{
		for( uint i = 0; i < it->second.size(); i++ )
		{
			const RenderablePtr& rend = it->second[i];
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = absoluteTransform;
			renderState.group = it->first;
			renderState.priority = 0;

			if( needsRenderCallback )
				renderState.callback.Connect( this, &Geometry::onRender );

			queue.push_back( renderState );
		}
	}
}

//-----------------------------------//

void Geometry::updateBounds()
{
	boundingVolume.reset();

	// Update the bounding box to accomodate new geometry.
	RenderableMap::const_iterator it;
	for( it = renderables.cbegin(); it != renderables.cend(); it++ )
	{
		for( uint i = 0; i < it->second.size(); i++ )
		{
			const RenderablePtr& rend = it->second[i];
			const VertexBufferPtr& vb = rend->getVertexBuffer();
			
			if( !vb ) continue;

			const std::vector<Vector3>& verts = vb->getVertices();
			for( uint e = 0; e < verts.size(); e++ )
			{
				const Vector3& vec = verts[e];
				boundingVolume.add(vec);
			}
		}
	}

	if( boundingVolume.isInfinite() )
		boundingVolume.setZero();
}

//-----------------------------------//

void Geometry::notifiesTransform()
{
	const EntityPtr& node = getEntity();
	assert( node != nullptr );

	const TransformPtr& transform = node->getTransform();
	assert( transform != nullptr );

	transform->markBoundingVolumeDirty();
}

//-----------------------------------//

void Geometry::update( double delta )
{
	if( !isDirty )
		return;

	updateBounds();
	notifiesTransform();
	isDirty = false;
}

//-----------------------------------//

BoundingBox Geometry::getWorldBoundingVolume() const
{
	const TransformPtr& trans = getEntity()->getTransform();
	const Matrix4x3& transform = trans->getAbsoluteTransform();
	return boundingVolume.transform(transform);
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

} // end namespace