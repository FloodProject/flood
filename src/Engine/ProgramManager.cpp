/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/ProgramManager.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

ProgramManager::ProgramManager()
{

}

//-----------------------------------//

ProgramManager::~ProgramManager()
{

}

//-----------------------------------//

ProgramPtr ProgramManager::getProgram( const std::string& name )
{
	ProgramPtr program = programs[name];

	if( !program )
	{
		// use a fallback default program
		// TODO: bundle fallback program in engine
		
		warn( "render::programs", 
			"Could not locate '%s': reverting to fallback program", name.c_str() );
	}

	return program;
}

//-----------------------------------//

void ProgramManager::registerProgram( const std::string& name, ProgramPtr program )
{
	// TODO: error handling
	programs[name] = program;
}

//-----------------------------------//

//uint TextureManager::getMemoryUsage()
//{
//	uint total = 0;
//
//	foreach( texPair p, textures )
//		total += p.first->getBuffer().size();
//
//	return total;
//}

//-----------------------------------//

} } // end namespaces