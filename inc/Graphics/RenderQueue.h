/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Matrix4x3.h"
#include "Math/Matrix4x4.h"
#include "Graphics/Renderable.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class Material;
class Renderable;
class Texture;
class Transform;
class Light;

/**
 * This contains all the rendering state information that is needed by
 * the rendering device to properly render the associated renderable.
 * This should be set by the camera and then appended to a render queue
 * where it will be sorted to minimize the state changes and finally
 * rendered to the render target that is currently active on the device.
 */

struct RenderState
{
	RenderState() {}
	RenderState( Renderable* renderable );
	
	RenderState( const RenderState& rhs )
		: renderable( rhs.renderable )
		, modelMatrix( rhs.modelMatrix )
		, material( rhs.material )
		, priority( rhs.priority )
	{ }

	Renderable* renderable;
	Material* material;
	Matrix4x3 modelMatrix;
	int32 priority;
};

//-----------------------------------//

/**
 * Light properties that will be passed down to the renderer.
 */

struct LightState
{
	Light* light;
	Transform* transform;
	Texture* depth;
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

NAMESPACE_ENGINE_END