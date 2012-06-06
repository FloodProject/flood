/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/Shader.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

Shader::Shader()
{
}

//-----------------------------------//

void Shader::forceRecompile()
{
	compiled = false;
}

//-----------------------------------//

bool Shader::isCompiled() const
{
	return compiled;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END