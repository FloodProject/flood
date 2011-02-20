/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Render/Program.h"

namespace vapor {

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

} // end namespace