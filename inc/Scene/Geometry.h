/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Transform.h"
#include "Math/BoundingBox.h"
#include "Render/Renderable.h"
#include "Render/RenderQueue.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

typedef std::vector<RenderablePtr> RenderableVector;

/**
 * This component holds geometry data for rendering. It can hold multiple
 * Renderable objects, each with different materials. When the camera node
 * traverses the scene graph, Geometry nodes are picked and passed to the
 * rendering device for rendering.
 */

REFLECT_DECLARE_CLASS(Geometry)

class API_ENGINE Geometry : public Component
{
	REFLECT_DECLARE_OBJECT(Geometry)

public:

	Geometry();
	Geometry( const RenderablePtr& renderable );

	// Adds a new renderable to this geometry.
	void addRenderable( const RenderablePtr& renderable );

	// Gets all the renderables in this geometry.
	RenderableVector getRenderables();

	// Appends all the renderables of this geometry to the queue.
	virtual void appendRenderables( RenderQueue& queue, const TransformPtr& transform );

	// Updates the geometry bounds if needed.
	virtual void updateBounds();

	// Updates some geometry information.
	void update( float delta ) OVERRIDE;

	// Gets the bounding volume of this geometry.
	GETTER(BoundingVolume, const BoundingBox&, bounds)

	// Gets the world bounding volume of the transform.
	BoundingBox getWorldBoundingVolume() const;

	// Marks the geometry as dirty (forces bounding volume update).
	void markDirty();

protected:

	// Notifies the Transform that bounding volumes are dirty.
	void notifiesTransform();

	// Renderables of the geometry.
	RenderableVector renderables;

	// Bounding volume of the geometry.
	BoundingBox bounds;

	// Keeps track if geometry has changed.
	bool isDirty;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Geometry );

//-----------------------------------//

NAMESPACE_ENGINE_END