/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/Renderable.h"
#include "vapor/math/Matrix4x3.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Use these different kinds of render groups to signal to the renderer
 * how you want your renderables to be sorted by the render device.
 * Lower numbers render before higher numbers.
 */

namespace RenderGroup
{
	enum Enum
	{
		Normal = 0,
		Overlays = 10
	};
}

//-----------------------------------//

/**
 * This contains all the rendering state information that is needed by
 * the rendering device to properly render the associated renderable.
 * This should be set by the camera and then appended to a render queue
 * where it will be sorted to minimize the state changes and finally
 * rendered to the render target that is currently active on the device.
 */

struct RenderState /*: private boost::noncopyable*/
{
	RenderState() : group( RenderGroup::Normal ), priority( 0 ) {}

	RenderablePtr renderable;
	math::Matrix4x3 modelMatrix;
	RenderGroup::Enum group;
	int priority;
};

//-----------------------------------//

/**
 * This is a queue of objects that are usually returned by performing
 * a culling operation on the camera. Each renderable can be assigned
 * a different priority to be rendered on different times. For example
 * you might want to render the GUI after everything else is rendered.
 */

typedef std::vector<RenderState> RenderQueue;

//-----------------------------------//

} } // end namespaces