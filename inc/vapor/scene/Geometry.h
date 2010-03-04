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

namespace vapor { namespace scene {

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
	Geometry( render::RenderablePtr rend );

	virtual ~Geometry();

	/// Adds a new renderable to this geometry.
	void addRenderable( render::RenderablePtr rend, 
		render::RenderGroup::Enum group = render::RenderGroup::Normal,
		uint priority = 0 );

	/// Gets all the renderables in this geometry.
	const std::vector< render::RenderablePtr >& getRenderables( 
		render::RenderGroup::Enum group = render::RenderGroup::Normal );

	/// Appends all the renderables of this geometry to the queue.
	void appendRenderables( render::RenderQueue& queue, TransformPtr transform );

	/// Updates the geometry if needed.
	virtual void update( double delta );

	/// Gets the bounding volume of this geometry.
	const math::AABB& getBoundingVolume() const;

	// Marks the geometry as dirty (forces AABB update).
	void markDirty();

	/// Returns the name of this component.
	virtual const std::string& getType() const;

	DECLARE_SERIALIZABLE();

protected:

	// Bounding volume of the geometry.
	math::AABB boundingVolume;
	bool isDirty;

	typedef std::vector< render::RenderablePtr > RenderableList;
	typedef std::pair<const  render::RenderGroup::Enum, RenderableList > rendPair;
	
	std::map< render::RenderGroup::Enum, RenderableList > renderables;
	
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Geometry );

//-----------------------------------//

} } // end namespaces