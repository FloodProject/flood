/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/RenderQueue.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

RenderState::RenderState( const RenderablePtr& rend )
	: renderable( rend.get() )
	, material( rend->getMaterial().Resolve() )
	, priority( rend->getRenderPriority() )
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END