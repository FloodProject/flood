/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Program.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

Program::Program( resources::ShaderPtr vs, resources::ShaderPtr ps )
	: linked( false )
{
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