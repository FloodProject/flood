/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/RenderBatch.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * This is a queue of objects that are usually returned by performing
 * a culling operation on the camera. Each renderable can be assigned
 * a different priority to be rendered on different times. For example
 * you might want to render the GUI after everything else is rendered.
 */

struct API_GRAPHICS RenderQueue
{
	std::vector<RenderBatch> batches;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END