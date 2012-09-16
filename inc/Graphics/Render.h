/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

API_GRAPHICS Allocator* GetRenderAllocator();
API_GRAPHICS Allocator* GetFrameAllocator();

API_GRAPHICS void RenderInitialize();
API_GRAPHICS void RenderDeinitialize();

//-----------------------------------//

NAMESPACE_GRAPHICS_END