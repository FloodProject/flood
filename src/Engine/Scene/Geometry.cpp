/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Scene/Geometry.h"
#include "Engine/Scene/Entity.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Geometry, Component)
REFLECT_CLASS_END()

//-----------------------------------//

Geometry::Geometry()
	: needsBoundsRebuild(true)
{ }

//-----------------------------------//

void Geometry::addRenderable(const RenderBatchPtr& rend)
{
	renderables.pushBack( rend );
}

//-----------------------------------//

RenderablesVector Geometry::getRenderables() const
{
	return renderables;
}

//-----------------------------------//
 
void Geometry::appendRenderables( RenderQueue& queue, const Transform* transform )
{
	const Matrix4x3& absoluteTransform = transform->getAbsoluteTransform();

	for( size_t i = 0; i < renderables.size(); i++ )
	{
		RenderBatch* renderable = renderables[i].get();
		if( !renderable ) continue;

		RenderState state( renderable );
		state.modelMatrix = absoluteTransform;

		queue.pushBack(state);
	}
}

//-----------------------------------//

void Geometry::updateBounds()
{
	bounds.reset();

	// Update the bounding box to accomodate new geometry.
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		Renderable* rend = renderables[i].get();
		
		GeometryBuffer* gb = rend->getGeometryBuffer().get();
		if( !gb ) continue;

		uint32 numVertices = gb->getNumVertices();
		
		for( size_t j = 0; j < numVertices; j++ )
		{
			Vector3* vertex = (Vector3*) gb->getAttribute(VertexAttribute::Position, j);
			bounds.add(*vertex);
		}
	}

	if( bounds.isInfinite() )
	{
		bounds.setZero();
	}

	needsBoundsRebuild = false;
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
	if( !needsBoundsRebuild ) return;

	updateBounds();
	notifiesTransform();
}

//-----------------------------------//

BoundingBox Geometry::getWorldBoundingVolume() const
{
	const TransformPtr& trans = getEntity()->getTransform();
	const Matrix4x3& transform = trans->getAbsoluteTransform();
	return bounds.transform(transform);
}

//-----------------------------------//

void Geometry::rebuildBoundingBox()
{
	needsBoundsRebuild = true;
}

//-----------------------------------//

NAMESPACE_ENGINE_END