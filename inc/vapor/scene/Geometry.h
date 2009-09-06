/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Node.h"

#include "vapor/render/Renderable.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

/**
 * Scene node that holds geometry data for rendering. It can holds various
 * Renderable objects and treat it as one big piece of geometry.
 */

class Geometry : public Node
{
public:

	Geometry( render::RenderablePtr rend );

	virtual ~Geometry();

	void addRenderable( render::RenderablePtr rend );

	const std::vector< render::RenderablePtr >& getRenderables() { return renderables; }

	virtual void update();

	virtual const std::string save(int indent = 0);

	virtual const std::string name() const;

private:

	std::vector< render::RenderablePtr > renderables;
};

//-----------------------------------//

typedef tr1::shared_ptr< Geometry > GeometryPtr;

//-----------------------------------//

} } // end namespaces