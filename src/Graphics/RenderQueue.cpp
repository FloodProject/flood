/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
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

NAMESPACE_GRAPHICS_END