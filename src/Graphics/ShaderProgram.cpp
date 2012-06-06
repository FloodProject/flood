/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
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