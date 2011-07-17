/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Program.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Program::Program()
	: linked( false )
{ }

//-----------------------------------//

bool Program::validateShaders() const
{
	const ShaderPtr& vs = getVertexShader();
	
	if( vs->getShaderType() != ShaderType::Vertex )
		return false;

	const ShaderPtr& fs = getFragmentShader();
	
	if( fs->getShaderType() != ShaderType::Fragment )
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