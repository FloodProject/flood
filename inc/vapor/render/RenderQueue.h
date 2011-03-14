/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Renderable.h"
#include "Scene/Transform.h"
#include "Scene/Light.h"
#include "Event.h"

namespace vapor {

//-----------------------------------//

/**
 * This contains all the rendering state information that is needed by
 * the rendering device to properly render the associated renderable.
 * This should be set by the camera and then appended to a render queue
 * where it will be sorted to minimize the state changes and finally
 * rendered to the render target that is currently active on the device.
 */

typedef Event0<> RenderDelegate;

struct RenderState
{
	RenderState() { }
	
	RenderState( const RenderState& rhs )
		: renderable( rhs.renderable )
		, modelMatrix( rhs.modelMatrix )
	{ }

	RenderablePtr renderable;
	Matrix4x3 modelMatrix;
};

//-----------------------------------//

/**
 * Light properties that will be passed down to the renderer.
 */

struct LightState
{
	LightPtr light;
	TransformPtr transform;
	TexturePtr depth;
	Matrix4x4 projection;
};

//-----------------------------------//

/**
 * This is a queue of objects that are usually returned by performing
 * a culling operation on the camera. Each renderable can be assigned
 * a different priority to be rendered on different times. For example
 * you might want to render the GUI after everything else is rendered.
 */

typedef std::vector<RenderState> RenderQueue;
typedef std::vector<LightState> LightQueue;

struct RenderBlock
{
	RenderQueue renderables;
	LightQueue lights;
};

//-----------------------------------//

} // end namespace