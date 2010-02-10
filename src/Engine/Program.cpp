/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Program.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

Program::Program( const ShaderPtr& vs, const ShaderPtr& ps )
	: linked( false )
{
	if( !vs || !ps ) return;

	assert( vs->getType() == ShaderType::Vertex );
	assert( ps->getType() == ShaderType::Fragment );
}

//-----------------------------------//

Program::~Program()
{

}

//-----------------------------------//

bool Program::isLinked() const
{
	return linked;
}

//-----------------------------------//

const std::string& Program::getLog() const
{
	return log;
}

//-----------------------------------//

} } // end namespaces