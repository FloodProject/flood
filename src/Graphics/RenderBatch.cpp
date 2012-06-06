/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderBatch.h"
#include "Graphics/RenderDevice.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderBatch* RenderBatchCreate( Allocator* alloc )
{
	RenderBatch* rend = Allocate(alloc, Renderable);
	return rend;
}

//-----------------------------------//

RenderBatchRange::RenderBatchRange()
	: start(0)
	, end(0)
{
}

//-----------------------------------//

RenderBatch::RenderBatch()
	: type( PrimitiveType::Triangles )
	, mode( PrimitiveRasterMode::Solid )
	, stage( RenderLayer::Normal )
	, priority(0)
{
	ub = AllocateRender(UniformBuffer);
}

//-----------------------------------//

RenderBatch::~RenderBatch()
{

}

//-----------------------------------//

NAMESPACE_GRAPHICS_END