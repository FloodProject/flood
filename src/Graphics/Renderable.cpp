/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/Renderable.h"
#include "Graphics/Device.h"
#include "Graphics/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Renderable* RenderableCreate( Allocator* alloc )
{
	Renderable* rend = Allocate(Renderable, alloc);
	return rend;
}

//-----------------------------------//

Renderable::Renderable()
	: type( PolygonType::Triangles )
	, mode( PolygonMode::Solid )
	, stage( RenderLayer::Normal )
	, priority(0)
{
	ub = Allocate(UniformBuffer, GetRenderAllocator());
}

//-----------------------------------//

Renderable::~Renderable()
{

}

//-----------------------------------//

NAMESPACE_ENGINE_END