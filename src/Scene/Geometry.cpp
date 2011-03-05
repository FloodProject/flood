/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Geometry.h"
#include "scene/Entity.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Geometry, Component)
END_CLASS()

//-----------------------------------//

Geometry::Geometry()
	: isDirty(true)
{ }

//-----------------------------------//

Geometry::Geometry( const RenderablePtr& rend )
	: isDirty(true)
{
	addRenderable( rend );
}

//-----------------------------------//

void Geometry::addRenderable(const RenderablePtr& rend)
{
	renderables.push_back( rend );
}

//-----------------------------------//

RenderableVector Geometry::getRenderables()
{
	return renderables;
}

//-----------------------------------//
 
void Geometry::appendRenderables( RenderQueue& queue, TransformPtr transform )
{
	if( !transform )
		return;
	
	const Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();

	for( uint i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		
		RenderState state;
		state.renderable = rend;
		state.modelMatrix = absoluteTransform;

		queue.push_back(state);
	}
}

//-----------------------------------//

void Geometry::updateBounds()
{
	bounds.reset();

	// Update the bounding box to accomodate new geometry.
	for( uint i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		const VertexBufferPtr& vb = rend->getVertexBuffer();
			
		if( !vb ) continue;

		const std::vector<Vector3>& verts = vb->getVertices();
		
		for( uint j = 0; j < verts.size(); j++ )
		{
			const Vector3& vertex = verts[j];
			bounds.add(vertex);
		}
	}

	if( bounds.isInfinite() )
		bounds.setZero();
}

//-----------------------------------//

void Geometry::notifiesTransform()
{
	const TransformPtr& transform = entity->getTransform();
	transform->markBoundingVolumeDirty();
}

//-----------------------------------//

void Geometry::update( float delta )
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
	return bounds.transform(transform);
}

//-----------------------------------//

void Geometry::markDirty()
{
	isDirty = true;
}

//-----------------------------------//

} // end namespace