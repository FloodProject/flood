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
#include "vapor/math/AABB.h"

namespace vapor {

//-----------------------------------//

/**
 * This component holds geometry data for rendering. It can hold multiple
 * Renderable objects, each with different materials. When the camera node
 * traverses the scene graph, Geometry nodes are picked and passed to the
 * rendering device for rendering.
 */

class VAPOR_API Geometry : public Component
{
public:

	Geometry();
	Geometry( RenderablePtr rend );

	/// Adds a new renderable to this geometry.
	void addRenderable( RenderablePtr rend, 
		RenderGroup::Enum group = RenderGroup::Normal,
		uint priority = 0 );

	/// Gets all the renderables in this geometry.
	const std::vector< RenderablePtr >& getRenderables( 
		RenderGroup::Enum group = RenderGroup::Normal );

	/// Appends all the renderables of this geometry to the queue.
	void appendRenderables( RenderQueue& queue, TransformPtr transform );

	/// Updates the geometry if needed.
	virtual void update( double delta );

	/// Gets the bounding volume of this geometry.
	GETTER(BoundingVolume, const AABB&, boundingVolume)

	// Marks the geometry as dirty (forces AABB update).
	void markDirty();

	/// Returns the name of this component.
	GETTER(Type, const std::string&, type)

	DECLARE_SERIALIZABLE();

protected:

	// Bounding volume of the geometry.
	AABB boundingVolume;
	bool isDirty;

	typedef std::vector< RenderablePtr > RenderableList;
	typedef std::pair<const RenderGroup::Enum, RenderableList > RenderableMapPair;
	typedef std::map< RenderGroup::Enum, RenderableList > RenderableMap;
	
	RenderableMap renderables;
	
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Geometry );
TYPEDEF_SHARED_POINTER_FROM_TYPE( Geometry );

//-----------------------------------//

} // end namespace