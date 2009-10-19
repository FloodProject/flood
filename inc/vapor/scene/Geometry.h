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
 * Scene node that holds geometry data for rendering. It can holds various
 * Renderable objects and treat it as one big piece of geometry.
 */

class VAPOR_API Geometry : public Transformable
{
public:

	Geometry();
	Geometry( render::RenderablePtr rend );

	virtual ~Geometry();

	// Adds a new renderable to this geometry.
	void addRenderable( render::RenderablePtr rend, 
		render::RenderGroup::Enum group = render::RenderGroup::Normal,
		uint priority = 0 );

	// Gets all the renderables in this geometry.
	const std::vector< render::RenderablePtr >& getRenderables( 
		render::RenderGroup::Enum group = render::RenderGroup::Normal );

	// Appends all the renderables of this geometry to the queue.
	void appendRenderables( render::RenderQueue& queue );

	// Updates the geometry if needed.
	virtual void update( double delta );

	// Serializes this node to a stream.
	virtual const std::string save(int indent = 0);

	// Returns the name of this node.
	virtual const std::string name() const;

protected:

	typedef std::vector< render::RenderablePtr > RenderableList;
	std::map< render::RenderGroup::Enum, RenderableList > renderables;
	typedef std::pair<const  render::RenderGroup::Enum, RenderableList > rendPair;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Geometry );

//-----------------------------------//

} } // end namespaces