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
typedef std::map< RenderStage::Enum, RenderableList > RenderableMap;
typedef std::pair<const RenderStage::Enum, RenderableList > RenderableMapPair;

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
	Geometry( const RenderablePtr& renderable );

	// Adds a new renderable to this geometry.
	void addRenderable( const RenderablePtr& renderable, 
		RenderStage::Enum group = RenderStage::Normal,
		uint priority = 0 );

	// Gets all the renderables in this geometry.
	const RenderableList& getRenderables( RenderStage::Enum = RenderStage::Normal );

	// Appends all the renderables of this geometry to the queue.
	void appendRenderables( RenderQueue& queue, TransformPtr transform );

	// Updates the geometry bounds if needed.
	virtual void updateBounds();

	// Updates some geometry information.
	virtual void update( double delta );

	// Callback when geometry is about to get rendered.
	virtual void onRender();

	// Gets the bounding volume of this geometry.
	GETTER(BoundingVolume, const BoundingBox&, boundingVolume)

	// Marks the geometry as dirty (forces bounding volume update).
	void markDirty();

protected:

	// Notifies the Transform that bounding volumes are dirty.
	void notifiesTransform();

	// Bounding volume of the geometry.
	BoundingBox boundingVolume;

	// Keeps track if geometry has changed.
	bool isDirty;

	// Keeps track if geometry needs render callback.
	bool needsRenderCallback;
	
	// Renderables of the geometry.
	RenderableMap renderables;
};

//-----------------------------------//

TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Geometry );
TYPEDEF_SHARED_POINTER_FROM_TYPE( Geometry );

//-----------------------------------//

} // end namespace