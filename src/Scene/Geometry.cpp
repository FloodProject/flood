/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Geometry.h"
#include "Scene/Entity.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Geometry, Component)
REFLECT_CLASS_END()

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
 
void Geometry::appendRenderables( RenderQueue& queue, const TransformPtr& transform )
{
	const Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();

	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& renderable = renderables[i];
		if( !renderable ) continue;

		RenderState state( renderable );
		state.modelMatrix = absoluteTransform;

		queue.push_back(state);
	}
}

//-----------------------------------//

void Geometry::updateBounds()
{
	bounds.reset();

	// Update the bounding box to accomodate new geometry.
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		const VertexBufferPtr& vb = rend->getVertexBuffer();
			
		if( !vb ) continue;

		const std::vector<Vector3>& verts = vb->getVertices();
		
		for( size_t j = 0; j < verts.size(); j++ )
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