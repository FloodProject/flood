/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderBatch* RenderBatchCreate( Allocator* alloc )
{
	RenderBatch* rend = Allocate(alloc, Renderable);
	return rend;
}

//-----------------------------------//

RenderBatch::RenderBatch()
{
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END