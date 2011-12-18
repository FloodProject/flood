/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderQueue.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderState::RenderState(RenderBatch* batch)
	: renderable( batch )
	, material( batch->getMaterial().Resolve() )
	, priority( batch->getRenderPriority() )
{ }

//-----------------------------------//

NAMESPACE_GRAPHICS_END