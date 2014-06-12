/************************************************************************
*
* vaporEngine by triton (2008-2010)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Matrix4x4.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class Material;
class RenderBatch;
class Texture;
class Transform;
class Light;

/**
 * This contains all the rendering state information that is needed by
 * the rendering device to properly render the associated.renderable.
 * This should be set by the camera and then appended to a render queue
 * where it will be sorted to minimize the state changes and finally
 * rendered to the render target that is currently active on the device.
 */

class API_GRAPHICS RenderState
{
public:

	RenderState();
	RenderState( RenderBatch* renderable );
	RenderState( const RenderState& rhs );

	RenderBatch* renderable;
	Material* material;
	Matrix4x3 modelMatrix;
	int32 priority;
};

//-----------------------------------//

/**
 * Light properties that will be passed down to the renderer.
 */

struct API_GRAPHICS FLD_IGNORE LightState
{
	Light* light;
	Transform* transform;
	Texture* depth;
	Matrix4x4 projection;
};

//-----------------------------------//

/**
 * This is a queue of objects that are usually returned by performing
 * a culling operation on the camera. Each.renderable can be assigned
 * a different priority to be rendered on different times. For example
 * you might want to render the GUI after everything else is rendered.
 */

typedef Vector<RenderState> RenderQueue;
typedef Vector<LightState> LightQueue;

struct API_GRAPHICS RenderBlock
{
	RenderQueue renderables;
	LightQueue lights;

    void addState(RenderState renderState);
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END