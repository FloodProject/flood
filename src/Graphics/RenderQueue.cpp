/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/RenderQueue.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

RenderState::RenderState( Renderable* rend )
	: renderable( rend )
	, material( rend->getMaterial().Resolve() )
	, priority( rend->getRenderPriority() )
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END