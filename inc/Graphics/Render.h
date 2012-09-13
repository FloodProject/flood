/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

API_RENDER Allocator* GetRenderAllocator();
API_RENDER Allocator* GetFrameAllocator();

API_RENDER void RenderInitialize();
API_RENDER void RenderDeinitialize();

//-----------------------------------//

NAMESPACE_GRAPHICS_END