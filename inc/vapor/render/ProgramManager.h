/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/ResourceManager.h"
#include "vapor/render/Program.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Manages a set of programs. The rest of the engine, when needing a 
 * program will ask the ProgramManager for the program with a name.
 * This way the engine will be able to provide, for example, fallback 
 * programs for when it is not possible to load the requested program.
 * This will be pretty useful when resource auto-loading is added, if a 
 * given file is corrupted we can safely render the scene, falling back
 * to default programs.
 */

class VAPOR_API ProgramManager : public Singleton<ProgramManager>
{
public:

	ProgramManager();
	~ProgramManager();

	// Gets a program given a name identifier.
	ProgramPtr getProgram( const std::string& program );

	void registerProgram( const std::string& name, ProgramPtr program );

private:

	// Maps the identifiers to the programs.
	std::map< std::string, ProgramPtr > programs;
	typedef std::pair< const std::string&, ProgramPtr > programPair;
	
};

//-----------------------------------//

} } // end namespaces