/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/Program.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Program::Program()
	: linked( false )
	, vertex( nullptr )
	, fragment( nullptr )
{ }

//-----------------------------------//

Program::~Program()
{
	LogDebug("Destroying program");
}

//-----------------------------------//

bool Program::validateShaders() const
{
	if( vertex->getShaderType() != ShaderType::Vertex )
		return false;

	if( fragment->getShaderType() != ShaderType::Fragment )
		return false;

	return true;
}

//-----------------------------------//

bool Program::isLinked() const
{
	return linked;
}

//-----------------------------------//

NAMESPACE_ENGINE_END