/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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