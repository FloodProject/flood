/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderBuffer::RenderBuffer(const Settings& settings)
	: settings( settings )
{ }

//-----------------------------------//

void RenderBuffer::makeCurrent()
{
	bind();
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END