/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Transform.h"
#include "vapor/render/RenderQueue.h"
#include "vapor/math/BoundingBox.h"

namespace vapor {

//-----------------------------------//

typedef std::vector< RenderablePtr > RenderableList;
typedef std::map< RenderGroup::Enum, RenderableList > RenderableMap;
typedef std::pair<const RenderGroup::Enum, RenderableList > RenderableMapPair;

//-----------------------------------//

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
	Geometry( RenderablePtr renderable );

	// Adds a new renderable to this geometry.
	void addRenderable( RenderablePtr renderable, 
		RenderGroup::Enum group = RenderGroup::Normal,
		uint priority = 0 );

	// Gets all the renderables in this geometry.
	const RenderableList& getRenderables( RenderGroup::Enum = RenderGroup::Normal );

	// Appends all the renderables of this geometry to the queue.
	void appendRenderables( RenderQueue& queue, TransformPtr transform );

	// Updates the geometry if needed.
	virtual void update( double delta );

	// Gets the bounding volume of this geometry.
	GETTER(BoundingVolume, const AABB&, boundingVolume)

	// Marks the geometry as dirty (forces bounding volume update).
	void markDirty();

protected:

	// Bounding volume of the geometry.
	AABB boundingVolume;

	// Keeps track if geometry has changed.
	bool isDirty;
	
	RenderableMap renderables;
};

//-----------------------------------//

TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Geometry );
TYPEDEF_SHARED_POINTER_FROM_TYPE( Geometry );

//-----------------------------------//

} // end namespace