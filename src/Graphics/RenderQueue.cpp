/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderQueue.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderState::RenderState()
	: RenderCommand(RenderCommandType::RenderState)
{
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END