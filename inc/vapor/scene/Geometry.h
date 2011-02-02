/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Transform.h"
#include "math/BoundingBox.h"
#include "render/Renderable.h"
#include "render/RenderQueue.h"

namespace vapor {

//-----------------------------------//

typedef std::vector<RenderablePtr> RenderableVector;

/**
 * This component holds geometry data for rendering. It can hold multiple
 * Renderable objects, each with different materials. When the camera node
 * traverses the scene graph, Geometry nodes are picked and passed to the
 * rendering device for rendering.
 */

class VAPOR_API Geometry : public Component
{
	DECLARE_CLASS_()

public:

	Geometry();
	Geometry( const RenderablePtr& renderable );

	// Adds a new renderable to this geometry.
	void addRenderable( const RenderablePtr& renderable );

	// Gets all the renderables in this geometry.
	RenderableVector getRenderables();

	// Appends all the renderables of this geometry to the queue.
	void appendRenderables( RenderQueue& queue, TransformPtr transform );

	// Updates the geometry bounds if needed.
	virtual void updateBounds();

	// Updates some geometry information.
	virtual void update( double delta );

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

TYPEDEF_SHARED_POINTER_FROM_TYPE( Geometry );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Geometry );

//-----------------------------------//

} // end namespace