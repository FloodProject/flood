/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Graphics/VertexBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

VertexBuffer::VertexBuffer() : built(false)
{
}

//-----------------------------------//

VertexBuffer::~VertexBuffer()
{
}

//-----------------------------------//

void VertexBuffer::forceRebuild()
{
	built = false;
}

//-----------------------------------//

bool VertexBuffer::isBuilt() const
{
	return built;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif