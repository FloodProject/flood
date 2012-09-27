/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"

#ifdef API_GRAPHICS_DLL_EXPORT
FL_INSTANTIATE_TEMPLATES()
#endif

NAMESPACE_GRAPHICS_BEGIN

static Allocator* gs_RenderAllocator = nullptr;
Allocator* GetRenderAllocator() { return gs_RenderAllocator; }

static Allocator* gs_FrameAllocator = nullptr;
Allocator* GetFrameAllocator() { return gs_FrameAllocator; }

void GraphicsInitialize()
{
	gs_RenderAllocator = AllocatorCreateHeap(AllocatorGetHeap());
	AllocatorSetGroup(gs_RenderAllocator, "Render");

	const int MEGABYTE = 1 << 20;

	gs_FrameAllocator = AllocatorCreateBump(gs_RenderAllocator, 1*MEGABYTE);
	AllocatorSetGroup(gs_FrameAllocator, "Frame");
}

void GraphicsDeinitialize()
{
	AllocatorDestroy(gs_FrameAllocator);
	AllocatorDestroy(gs_RenderAllocator);
}

NAMESPACE_GRAPHICS_END