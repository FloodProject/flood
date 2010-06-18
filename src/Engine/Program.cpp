/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Program.h"

namespace vapor {

//-----------------------------------//

Program::Program()
	: linked( false )
{ }

//-----------------------------------//

bool Program::validateShaders() const
{
	const ShaderPtr& vs = getVertexShader();
	
	if( vs->getType() != ShaderType::Vertex )
		return false;

	const ShaderPtr& fs = getFragmentShader();
	
	if( fs->getType() != ShaderType::Fragment )
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