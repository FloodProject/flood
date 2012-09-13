/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/ShaderProgram.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

ShaderProgram::ShaderProgram()
	: linked( false )
	, vertex( nullptr )
	, fragment( nullptr )
{ }

//-----------------------------------//

ShaderProgram::~ShaderProgram()
{
	LogDebug("Destroying program");
}

//-----------------------------------//

bool ShaderProgram::validateShaders() const
{
	if( vertex->getShaderType() != ShaderType::Vertex )
		return false;

	if( fragment->getShaderType() != ShaderType::Fragment )
		return false;

	return true;
}

//-----------------------------------//

bool ShaderProgram::isLinked() const
{
	return linked;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END