/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Transform.h"

#include "vapor/render/Renderable.h"
#include "vapor/render/RenderQueue.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

/**
 * This component holds geometry data for rendering. It can holds multiple
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
	void appendRenderables( render::RenderQueue& queue );

	/// Updates the geometry if needed.
	virtual void update( float delta );

	/// Serializes this component to a stream.
	virtual const std::string save(int indent = 0);

	/// Returns the name of this component.
	virtual const std::string& getType() const;

protected:

	typedef std::vector< render::RenderablePtr > RenderableList;
	std::map< render::RenderGroup::Enum, RenderableList > renderables;
	typedef std::pair<const  render::RenderGroup::Enum, RenderableList > rendPair;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Geometry );

//-----------------------------------//

} } // end namespaces