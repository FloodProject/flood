/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Render/RenderBuffer.h"

namespace vapor {

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

} // end namespace