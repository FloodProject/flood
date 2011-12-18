/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Graphics/VertexBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

VertexBuffer::VertexBuffer() : built(false), gb(nullptr)
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