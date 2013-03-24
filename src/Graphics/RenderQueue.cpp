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
{
}

//-----------------------------------//

RenderState::RenderState(RenderBatch* batch)
	: renderable( batch )
	, material( batch->getMaterial().Resolve() )
	, priority( batch->getRenderPriority() )
{
}

//-----------------------------------//

RenderState::RenderState(const RenderState& rhs)
	: renderable( rhs.renderable )
	, modelMatrix( rhs.modelMatrix )
	, material( rhs.material )
	, priority( rhs.priority )
{
}

//-----------------------------------//

void RenderBlock::addState(RenderState renderState)
{
    renderables.push_back(renderState);
}

NAMESPACE_GRAPHICS_END