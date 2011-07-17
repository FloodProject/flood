/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

API_RENDER Allocator* GetRenderAllocator();
API_RENDER Allocator* GetFrameAllocator();

API_RENDER void RenderInitialize();
API_RENDER void RenderDeinitialize();

//-----------------------------------//

NAMESPACE_ENGINE_END