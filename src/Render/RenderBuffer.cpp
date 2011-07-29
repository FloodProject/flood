/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/RenderBuffer.h"

NAMESPACE_ENGINE_BEGIN

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

NAMESPACE_ENGINE_END